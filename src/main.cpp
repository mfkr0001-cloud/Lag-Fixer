#include <Geode/Geode.hpp>
#include <Geode/modify/CCParticleSystemQuad.hpp>
#include <cstdint>

using namespace geode::prelude;

// Rendering-only workaround. Selected particle draw calls are skipped; particle
// simulation/update, player physics, collision, hitboxes, and level timing are
// not intentionally modified. Nonzero levels may still cause visible flicker.
class $modify(LagReducerParticleSystem, cocos2d::CCParticleSystemQuad) {
    struct Fields {
        std::uint32_t drawCounter = 0;
        bool phaseInitialized = false;
    };

    void draw() {
        auto* mod = Mod::get();

        // Master switch: restore normal rendering when optimization is disabled.
        if (!mod->getSettingValue<bool>("enabled")) {
            cocos2d::CCParticleSystemQuad::draw();
            return;
        }

        const auto reduction = mod->getSettingValue<std::int64_t>("particle-draw-reduction");
        if (reduction > 0) {
            // Give each particle system a different starting phase. This avoids
            // every system intentionally skipping on the same frame, which can
            // make the whole screen flash together. It does not remove all flicker.
            if (!m_fields->phaseInitialized) {
                const auto address = reinterpret_cast<std::uintptr_t>(this);
                m_fields->drawCounter = static_cast<std::uint32_t>(address >> 4);
                m_fields->phaseInitialized = true;
            }

            // Levels 1..4 skip one out of every 2..5 draw calls per system.
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
    log::info("Lag Reducer v1.2.4 loaded. Particle draw reduction defaults to level 0.");
}
