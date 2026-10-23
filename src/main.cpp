#include <Geode/Geode.hpp>
#include <Geode/modify/CCParticleSystem.hpp>
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
// Cached in memory so particle-system updates do not repeatedly query Geode settings.
bool g_enabled = true;
std::int64_t g_strength = 0;

// Fraction of each emitter's original maximum particle budget retained at each setting.
// 0 restores normal behaviour. Higher strengths lower both the emission rate and the
// maximum number of simultaneously active particles. Unlike draw-call skipping, this
// does not deliberately make visible particles blink on/off every few frames.
constexpr std::array<float, 11> kParticleBudget = {
    1.00f, 0.90f, 0.82f, 0.74f, 0.66f, 0.58f,
    0.50f, 0.42f, 0.34f, 0.27f, 0.20f
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
    // Keep a non-zero capacity for small emitters, even at maximum strength.
    return std::clamp(static_cast<int>(std::ceil(original * keep)), 1, original);
}
}

// Particle-only performance optimization. This changes only emitter budgets/rates;
// it does not hook player physics, collision handling, hitboxes, or level timing.
// If the system currently has more live particles than the new cap, wait for the count
// to fall to a safe level before shrinking its allocation. Lower emission rate is applied
// immediately; the particle cap is applied when safe, avoiding a mid-frame array shrink.
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
            m_fields->originalCapacity = std::max(0, static_cast<int>(this->getTotalParticles()));
            m_fields->originalEmissionRate = this->getEmissionRate();
            m_fields->requestedEmissionRate = m_fields->originalEmissionRate;
        }

        const auto strength = activeStrength();
        if (strength != m_fields->appliedStrength) {
            const float keep = keepFraction(strength);
            m_fields->requestedCapacity = scaledCapacity(m_fields->originalCapacity, keep);
            m_fields->requestedEmissionRate = m_fields->originalEmissionRate * keep;
            this->setEmissionRate(m_fields->requestedEmissionRate);
            m_fields->capacityPending =
                this->getTotalParticles() != m_fields->requestedCapacity;
            m_fields->appliedStrength = static_cast<int>(strength);
        }

        if (m_fields->capacityPending) {
            const int target = m_fields->requestedCapacity;
            const auto liveCount = static_cast<int>(this->getParticleCount());
            if (liveCount <= target) {
                this->setTotalParticles(target);
                // Some Cocos2d implementations recalculate emission rate when the cap changes.
                this->setEmissionRate(m_fields->requestedEmissionRate);
                m_fields->capacityPending = false;
            }
        }

        cocos2d::CCParticleSystem::update(dt);
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

    listenForSettingChanges("enabled", [](bool value) {
        g_enabled = value;
    });
    listenForSettingChanges("particle-draw-reduction", [](std::int64_t value) {
        g_strength = std::clamp<std::int64_t>(value, 0, 10);
    });

    log::info("Lag Reducer v1.2.8 loaded. Particle budgets replace per-frame draw skipping; pause settings shortcut enabled.");
}
