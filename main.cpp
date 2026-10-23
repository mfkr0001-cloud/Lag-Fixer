#include <Geode/Geode.hpp>
#include <Geode/modify/CCParticleSystemQuad.hpp>

using namespace geode::prelude;

// Rendering-only optimization. Particle simulation/update, collision, player
// physics, and level timing are not modified. A nonzero value may cause flicker.
class $modify(LagReducerParticleSystem, cocos2d::CCParticleSystemQuad) {
    struct Fields {
        // Unsigned wraparound is defined and avoids signed overflow in long sessions.
        uint32_t drawCounter = 0;
    };

    void draw() {
        const auto reduction = Mod::get()->getSettingValue<int64_t>("particle-draw-reduction");
        if (reduction > 0) {
            // Values 1..3 skip 1 of every 2..4 draw calls for each system.
            const int interval = static_cast<int>(reduction) + 1;
            const bool skipThisDraw = (m_fields->drawCounter % static_cast<uint32_t>(interval)) == static_cast<uint32_t>(interval - 1);
            ++m_fields->drawCounter;
            if (skipThisDraw) {
                return;
            }
        }
        cocos2d::CCParticleSystemQuad::draw();
    }
};

$on_mod(Loaded) {
    log::info("Lag Reducer 1.2.0 loaded. Particle draw reduction defaults to off.");
}
