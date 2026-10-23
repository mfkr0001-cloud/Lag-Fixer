Lag Reducer — Geode v1.2.6
A Geometry Dash / Geode mod that reduces cosmetic particle workload to try to improve frame stability in effect-heavy levels.
Settings
Enable particle optimization: master on/off switch.
Particle optimization strength (`particle-draw-reduction`):
`0`: normal particle rendering and updates.
`1–3`: progressively fewer particle draw calls; simulation still updates every call.
`4–7`: particle simulation updates every 2 calls using accumulated elapsed time.
`8–10`: particle simulation updates every 3 calls; `10` draws approximately 1 in every 11 calls.
The previous configuration key is retained so the existing setting stays compatible. After installing this build, set strength to `10` manually to test maximum optimization.
Important limitations
This is an experimental particle optimization, not a guaranteed FPS fix. It does not hook player physics, collisions, hitboxes, or level timing. Particle effects can flicker or look choppy at high values. If frame drops are caused by other game systems, device throttling, or frame pacing, the player may still appear to jump forward. Use the lowest strength that helps, and compare with `0` if timing or music feels wrong.
Android build using GitHub Actions
Upload the project contents to your `Lag-Fixer` GitHub repository, including `.github` and `logo.png`.
Push a commit or open Actions → Build Lag Reducer (Android).
If the workflow succeeds, download the `LagReducer-Android64` artifact. It should contain the `.geode` file.
The workflow uses `sdk: nightly`; `mod.json` records Geode `5.11.2` based on the previous successful build.
Build verification
This ZIP is a source project, not a compiled `.geode` package. This version has not been compiled or tested in Geometry Dash in this environment; run GitHub Actions before installing it.
