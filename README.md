Lag Reducer — Geode v1.2.3
An Android64-focused Geometry Dash / Geode mod with a configurable rendering workaround.
Features
Master switch to restore normal particle drawing without changing the selected strength.
Particle draw reduction with five levels. The default is level 0, which makes no intentional rendering changes.
Level 1: skips 1 of every 2 particle draw calls per system.
Level 2: skips 1 of every 3.
Level 3: skips 1 of every 4.
Level 4: skips 1 of every 5.
This skips selected draw calls; it is not a proven FPS fix and is not a particle-count reducer. Nonzero levels can cause flicker, hide visual cues, or make effects look incorrect. Particle simulation/update is not skipped. The mod does not intentionally change gameplay physics, hitboxes, or level timing. Start at level 0 and increase only if you accept the visual tradeoff.
Android build with GitHub Actions
Upload the contents of this folder to the `Lag-Fixer` GitHub repository, including the `.github` folder and `logo.png`.
In `mod.json`, replace `REPLACE_WITH_YOUR_USERNAME` in the `repository` URL with the actual GitHub username.
Push a commit or open Actions → Build Lag Reducer (Android).
If the workflow succeeds, download the `LagReducer-Android64` artifact. It contains the `.geode` file.
The workflow uses `sdk: given` to align the selected SDK with the version in `mod.json`.
Verification notes
The previous CI log failed while compiling the `fmt` dependency under Clang 21, before it reported any error from this mod's own `main.cpp`. `FMT_USE_CONSTEVAL=0` is retained as a workaround attempt, not a guaranteed fix. The source in this archive has not been compiled against the actual Android64 Geode SDK here. A successful Actions run is required before treating it as build-verified.
