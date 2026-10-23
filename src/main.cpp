#include <Geode/Geode.hpp>
#include <Geode/modify/CCParticleSystemQuad.hpp>
#include <cstdint>

using namespace geode::prelude;

// Rendering-only workaround. This hook skips selected particle-system draw calls.
// Particle simulation/update, player physics, collisions, hitboxes, and level timing
// are not intentionally modified. Stronger levels can make particles flicker.
class $modify(LagReducerParticleSystem, cocos2d::CCParticleSystemQuad) {
    struct Fields {
        std::uint32_t drawCounter = 0;
        bool phaseInitialized = false;
    };

    void draw() {
        auto* mod = Mod::get();

        // Restore normal drawing when the master switch is off.
        if (!mod->getSettingValue<bool>("enabled")) {
            cocos2d::CCParticleSystemQuad::draw();
            return;
        }

        const auto reduction = mod->getSettingValue<std::int64_t>("particle-draw-reduction");
        if (reduction > 0) {
            // Offset each emitter so all particle systems are less likely to skip
            // their draw on the same frame.
            if (!m_fields->phaseInitialized) {
                const auto address = reinterpret_cast<std::uintptr_t>(this);
                m_fields->drawCounter = static_cast<std::uint32_t>(address >> 4);
                m_fields->phaseInitialized = true;
            }

            // Correct strength ordering:
            // 1 draws 1/2 calls; 2 draws 1/3; 3 draws 1/4; 4 draws 1/5.
            // Higher settings skip more calls and may cause stronger flicker.
            const auto interval = static_cast<std::uint32_t>(reduction + 1);
            const bool drawThisCall = (m_fields->drawCounter % interval) == 0;
            ++m_fields->drawCounter;
            if (!drawThisCall) {
                return;
            }
        }

        cocos2d::CCParticleSystemQuad::draw();
    }
};

$on_mod(Loaded) {
    log::info("Lag Reducer v1.2.5 loaded. Higher reduction levels skip more particle draw calls.");
}
