#include <Geode/Geode.hpp>
#include <Geode/modify/CCParticleSystem.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/binding/ButtonSprite.hpp>
#include <Geode/binding/CCMenuItemSpriteExtra.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include <algorithm>
#include <array>
#include <cstdint>
#include <cmath>

using namespace geode::prelude;

namespace {
// Fast in-memory cache for the hot CCParticleSystem::update hook.
bool g_enabled = true;
std::int64_t g_strength = 0;

// Remaining fraction of each particle emitter's original budget at strengths 0..10.
// At 10, supported emitters retain at most about 1% (with at least one slot for small emitters).
constexpr std::array<float, 11> kParticleBudget = {
    1.00f, 0.80f, 0.65f, 0.50f, 0.38f, 0.28f,
    0.18f, 0.10f, 0.06f, 0.025f, 0.01f
};

std::int64_t activeStrength() {
    return g_enabled ? std::clamp<std::int64_t>(g_strength, 0, 10) : 0;
}

float keepFraction(std::int64_t strength) {
    return kParticleBudget[static_cast<std::size_t>(
        std::clamp<std::int64_t>(strength, 0, 10)
    )];
}

int scaledCapacity(int original, float keep) {
    if (original <= 0) return 0;
    return std::clamp(static_cast<int>(std::ceil(original * keep)), 1, original);
}

// Apply the game's visual-performance switches before levels are entered. The baseline
// values are saved once so disabling this mode can restore the player's previous settings.
void applyGameVisualSettings() {
    auto* mod = Mod::get();
    auto* gm = GameManager::sharedState();
    if (!mod || !gm) return;

    // Capture vanilla values before this mod changes either game variable. Persist them so
    // a later toggle/relaunch can restore the same baseline rather than blindly enabling it.
    if (!mod->getSavedValue<bool>("lr-originals-captured", false)) {
        mod->setSavedValue<bool>("lr-original-glow", gm->getGameVariable("0058"));
        mod->setSavedValue<bool>("lr-original-particles", gm->getGameVariable("0043"));
        mod->setSavedValue<bool>("lr-originals-captured", true);
    }

    const bool baselineGlow = mod->getSavedValue<bool>("lr-original-glow", true);
    const bool baselineParticles = mod->getSavedValue<bool>("lr-original-particles", true);
    const bool ultraLow = mod->getSettingValue<bool>("ultra-low");
    const bool disableGlow = ultraLow && mod->getSettingValue<bool>("disable-glow");
    const bool hideParticles = ultraLow && mod->getSettingValue<bool>("hide-particles");

    gm->setGameVariable("0058", disableGlow ? false : baselineGlow);
    gm->setGameVariable("0043", hideParticles ? false : baselineParticles);

    if (mod->getSettingValue<bool>("performance-logs")) {
        log::info(
            "Lag Reducer v1.3.0 visual mode applied (ultra-low={}, disable-glow={}, hide-particles={})",
            ultraLow, disableGlow, hideParticles
        );
    }
}
}

// Reduce particle simulation/emission load. This never hooks player physics, collisions,
// hitboxes, level triggers, or timing. If an emitter is not fully initialized yet, leave it
// untouched and try again on its next update rather than permanently caching a zero capacity.
class $modify(LagReducerParticleBudget, cocos2d::CCParticleSystem) {
    struct Fields {
        int originalCapacity = -1;
        float originalEmissionRate = 0.0f;
        float requestedEmissionRate = 0.0f;
        int appliedStrength = -1;
        int requestedCapacity = -1;
        bool capacityPending = false;
    };

    void update(float dt) {
        if (m_fields->originalCapacity < 0) {
            const int initialCapacity = static_cast<int>(this->getTotalParticles());
            if (initialCapacity > 0) {
                m_fields->originalCapacity = initialCapacity;
                m_fields->originalEmissionRate = this->getEmissionRate();
                m_fields->requestedEmissionRate = m_fields->originalEmissionRate;
            }
        }

        if (m_fields->originalCapacity < 0) {
            cocos2d::CCParticleSystem::update(dt);
            return;
        }

        const auto strength = activeStrength();
        if (strength != m_fields->appliedStrength) {
            const float keep = keepFraction(strength);
            m_fields->requestedCapacity = scaledCapacity(m_fields->originalCapacity, keep);
            m_fields->requestedEmissionRate = m_fields->originalEmissionRate * keep;
            this->setEmissionRate(m_fields->requestedEmissionRate);
            m_fields->capacityPending =
                static_cast<int>(this->getTotalParticles()) != m_fields->requestedCapacity;
            m_fields->appliedStrength = static_cast<int>(strength);
        }

        if (m_fields->capacityPending) {
            const int target = m_fields->requestedCapacity;
            const int liveCount = static_cast<int>(this->getParticleCount());
            if (liveCount <= target) {
                this->setTotalParticles(static_cast<unsigned int>(target));
                this->setEmissionRate(m_fields->requestedEmissionRate);
                m_fields->capacityPending = false;
            }
        }

        cocos2d::CCParticleSystem::update(dt);
    }
};

// Apply the low-graphics options when the game/menu is constructed and again before a level.
class $modify(LagReducerMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;
        applyGameVisualSettings();
        return true;
    }
};

class $modify(LagReducerPlayLayer, PlayLayer) {
    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects)) return false;
        applyGameVisualSettings();
        return true;
    }
};

// Pause-menu shortcut: opens the regular Geode settings popup without leaving the level.
class $modify(LagReducerPauseLayer, PauseLayer) {
    void customSetup() {
        PauseLayer::customSetup();

        const auto winSize = CCDirector::sharedDirector()->getWinSize();
        auto menu = CCMenu::create();
        if (!menu) return;

        auto sprite = ButtonSprite::create(
            "Lag Reducer", "bigFont.fnt", "GJ_button_04.png", 0.7f
        );
        if (!sprite) return;
        sprite->setScale(0.55f);

        auto button = CCMenuItemSpriteExtra::create(
            sprite, this, menu_selector(LagReducerPauseLayer::onOpenLagReducerSettings)
        );
        if (!button) return;

        menu->addChild(button);
        menu->setPosition({winSize.width - 84.f, winSize.height - 32.f});
        this->addChild(menu, 100);
    }

    void onOpenLagReducerSettings(CCObject*) {
        geode::openSettingsPopup(Mod::get());
    }
};

$on_mod(Loaded) {
    auto* mod = Mod::get();
    g_enabled = mod->getSettingValue<bool>("enabled");
    g_strength = std::clamp<std::int64_t>(
        mod->getSettingValue<std::int64_t>("particle-draw-reduction"), 0, 10
    );

    listenForSettingChanges<bool>("enabled", [](bool value) {
        g_enabled = value;
    });
    listenForSettingChanges<std::int64_t>("particle-draw-reduction", [](std::int64_t value) {
        g_strength = std::clamp<std::int64_t>(value, 0, 10);
    });

    listenForSettingChanges<bool>("ultra-low", [](bool) {
        applyGameVisualSettings();
    });
    listenForSettingChanges<bool>("disable-glow", [](bool) {
        applyGameVisualSettings();
    });
    listenForSettingChanges<bool>("hide-particles", [](bool) {
        applyGameVisualSettings();
    });
    listenForSettingChanges<bool>("performance-logs", [](bool) {
        applyGameVisualSettings();
    });

    log::info("Lag Reducer v1.3.0 loaded. Particle budget optimization and low-graphics switches are ready.");
}
