# Lag Reducer — Geode 1.2.0

An Android64-focused Geometry Dash / Geode mod that can skip selected particle-system draw calls to reduce some rendering work.

## Settings

**Particle draw reduction** ranges from 0 to 3:

- `0` — disabled (default; normal particle rendering)
- `1` — skip one draw call in every two for each hooked particle system
- `2` — skip one draw call in every three
- `3` — skip one draw call in every four

This is a coarse rendering workaround, not a particle-count reducer. Nonzero values can make effects flicker and may hide visual cues. Particle simulation/update is deliberately not skipped. The mod does not intentionally modify player physics, hitboxes, or level timing.

## Android build with GitHub Actions

1. Upload the contents of this folder to a GitHub repository (including `.github`).
2. Replace `REPLACE_WITH_YOUR_USERNAME` in `mod.json` with the repository owner's GitHub username, or set the repository URL to your own repo.
3. Push to GitHub or run **Actions → Build Lag Reducer (Android) → Run workflow**.
4. If the workflow succeeds, download the `LagReducer-Android64` artifact. Install the `.geode` file using your normal Geode mod installation workflow.

The workflow uses `sdk: given` to align the SDK selected by the build action with the exact `geode` version in `mod.json`, rather than silently selecting a potentially different SDK release.

## Verification status and compatibility note

The project files and JSON/workflow syntax can be checked locally, but this environment does not have the Geode SDK / Android NDK toolchain installed, so an actual C++ Android build has **not** been run here. Compilation still depends on the availability of `Geode/modify/CCParticleSystemQuad.hpp` and the exact `draw()` hook binding in the Geode v4.0.0 SDK. The draw counter uses Geode’s documented `Fields` mechanism for state in `$modify` classes. GitHub Actions is retained as the real build gate; a successful run is needed before claiming this source compiles.

No claim is made that this guarantees higher FPS on every device. Test with reduction disabled first, then increase it gradually and turn it back down if effects flicker or important cues disappear.
