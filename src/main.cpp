#include <Geode/Geode.hpp>
#include <Geode/modify/CCParticleSystemQuad.hpp>
#include <cstdint>

using namespace geode::prelude;

// Rendering-only workaround. This hook skips selected particle draw calls;
// it does not skip particle simulation/update and does not intentionally alter
// player physics, collision, hitboxes, or level timing. Some effects can flicker.
class $modify(LagReducerParticleSystem, cocos2d::CCParticleSystemQuad) {
    struct Fields {
        // Per-particle-system counter; unsigned wraparound is defined.
        std::uint32_t drawCounter = 0;
    };

    void draw() {
        auto* mod = Mod::get();

        // Master switch: preserve normal rendering when optimization is disabled.
        if (!mod->getSettingValue<bool>("enabled")) {
            cocos2d::CCParticleSystemQuad::draw();
            return;
        }

        const auto reduction = mod->getSettingValue<std::int64_t>("particle-draw-reduction");
        if (reduction > 0) {
            // Levels 1..4 skip 1 out of every 2..5 draw calls per system.
            const auto interval = static_cast<std::uint32_t>(reduction + 1);
            const bool skipThisDraw =
                (m_fields->drawCounter % interval) == (interval - 1);
            ++m_fields->drawCounter;
            if (skipThisDraw) {
                return;
            }
        }

        cocos2d::CCParticleSystemQuad::draw();
    }
};

$on_mod(Loaded) {
    log::info("Lag Reducer v1.2.3 loaded. Particle draw reduction defaults to level 0.");
}
