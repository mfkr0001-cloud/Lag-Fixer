Lag Reducer — Geode v1.2.4
An Android64-focused Geometry Dash / Geode mod with a configurable particle-rendering workaround.
Features
Master switch to restore normal particle drawing without changing the selected strength.
Particle draw reduction with five levels. Level 0 is the default and makes no intentional rendering changes.
Level 1: skips 1 of every 2 particle draw calls per system.
Level 2: skips 1 of every 3.
Level 3: skips 1 of every 4.
Level 4: skips 1 of every 5.
Starting phases are staggered per particle system so that systems are less likely to skip on the same frame.
This is a rendering workaround, not a proven FPS fix or particle-count reducer. Nonzero levels can cause flicker, hide visual cues, or make effects look incorrect. Particle simulation/update is not skipped. The mod does not intentionally change gameplay physics, hitboxes, or level timing. Leave level 0 if visual quality is more important than experimenting with reduced draw load.
Android build with GitHub Actions
Upload the contents of this folder to the `Lag-Fixer` GitHub repository, including `.github` and `logo.png`.
Replace `REPLACE_WITH_YOUR_USERNAME` in the `repository` URL in `mod.json` with the actual GitHub username.
Push a commit or open Actions → Build Lag Reducer (Android).
If the workflow succeeds, download the `LagReducer-Android64` artifact. It contains the `.geode` file.
The workflow uses `sdk: nightly`; the `geode` metadata currently matches the SDK version reported by the last successful build, `5.11.2`. If the workflow's nightly SDK changes in the future, the metadata may need to be updated again.
Verification
The previous CI build succeeded before this v1.2.4 source update. The new source must be built in GitHub Actions before treating this update as verified. The mod uses draw-call skipping; any performance improvement depends on the level and device and is not guaranteed.
