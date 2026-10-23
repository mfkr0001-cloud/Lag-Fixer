#include <Geode/Geode.hpp>
#include <Geode/modify/CCParticleSystem.hpp>
#include <Geode/modify/CCParticleSystemQuad.hpp>
#include <cstdint>

using namespace geode::prelude;

// This optimization targets cosmetic particle workloads only.
// It does not hook player physics, collisions, hitboxes, or level timing.
// Higher settings reduce particle drawing more aggressively and batch particle updates.
class $modify(LagReducerParticleSimulation, cocos2d::CCParticleSystem) {
    struct Fields {
        std::uint32_t updateCounter = 0;
        float accumulatedDt = 0.0f;
    };

    void update(float dt) {
        auto* mod = Mod::get();
        const bool enabled = mod->getSettingValue<bool>("enabled");
        const auto strength = mod->getSettingValue<std::int64_t>("particle-draw-reduction");

        if (!enabled || strength <= 0) {
            m_fields->updateCounter = 0;
            m_fields->accumulatedDt = 0.0f;
            cocos2d::CCParticleSystem::update(dt);
            return;
        }

        // Keep particle simulation fairly frequent even at maximum strength:
        // levels 1-3 update every call, 4-7 every 2 calls, 8-10 every 3 calls.
        // Accumulate elapsed time so particle lifetimes still advance approximately correctly.
        const std::uint32_t updateInterval = strength >= 8 ? 3u : (strength >= 4 ? 2u : 1u);
        m_fields->accumulatedDt += dt;
        const bool updateThisCall = (m_fields->updateCounter++ % updateInterval) == 0;
        if (updateThisCall) {
            const float elapsed = m_fields->accumulatedDt;
            m_fields->accumulatedDt = 0.0f;
            cocos2d::CCParticleSystem::update(elapsed);
        }
    }
};

class $modify(LagReducerParticleDrawing, cocos2d::CCParticleSystemQuad) {
    struct Fields {
        std::uint32_t drawCounter = 0;
        bool phaseInitialized = false;
    };

    void draw() {
        auto* mod = Mod::get();
        if (!mod->getSettingValue<bool>("enabled")) {
            cocos2d::CCParticleSystemQuad::draw();
            return;
        }

        const auto strength = mod->getSettingValue<std::int64_t>("particle-draw-reduction");
        if (strength <= 0) {
            cocos2d::CCParticleSystemQuad::draw();
            return;
        }

        // Level 1 draws 1/2 calls, level 4 draws 1/5, and level 10 draws 1/11.
        const auto interval = static_cast<std::uint32_t>(strength + 1);
        if (!m_fields->phaseInitialized) {
            const auto address = reinterpret_cast<std::uintptr_t>(this);
            m_fields->drawCounter = static_cast<std::uint32_t>(address >> 4);
            m_fields->phaseInitialized = true;
        }

        const bool drawThisCall = (m_fields->drawCounter++ % interval) == 0;
        if (!drawThisCall) return;
        cocos2d::CCParticleSystemQuad::draw();
    }
};

$on_mod(Loaded) {
    log::info("Lag Reducer v1.2.6 loaded. Particle optimization strength supports levels 0-10.");
}
