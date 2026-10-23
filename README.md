Lag Reducer — Geode v1.2.5
A Geometry Dash / Geode mod that reduces particle-system drawing to try to lower rendering load. The source is intended for Android64; the metadata also retains the Windows GD target.
Settings
Enable particle optimization: master on/off switch.
Particle draw reduction:
`0`: normal particle drawing.
`1`: draw about 1 out of every 2 particle draw calls.
`2`: draw about 1 out of every 3.
`3`: draw about 1 out of every 4.
`4`: draw about 1 out of every 5.
Version 1.2.5 fixes the strength direction from the previous source: higher numbers now skip more draw calls. Different particle systems use different starting phases so they are less likely to flicker together.
Important limitations
This is a rendering-only workaround, not a guaranteed FPS fix. It does not intentionally change player physics, collisions, hitboxes, particle simulation/update, or level timing. If Geometry Dash still drops frames because of another bottleneck, the player can still appear to jump forward during a hitch. Values `2–4` can cause obvious particle flicker or hide visual cues; use the lowest value that helps. If a level's music/visual timing seems wrong, test with `0` and compare.
Android build using GitHub Actions
Upload the project contents to your `Lag-Fixer` GitHub repository, including `.github` and `logo.png`.
Push a commit or open Actions → Build Lag Reducer (Android).
If the workflow succeeds, download the `LagReducer-Android64` artifact. It should contain the `.geode` file.
The CI workflow uses `sdk: nightly`, while `mod.json` records the Geode SDK version (`5.11.2`) from the previous successful build. If the nightly SDK version changes, this value may need to be updated to match it.
Build verification
The edited source package is not itself a compiled `.geode` mod. Run the GitHub Actions build before installing it; this update has not been compiled or device-tested in this environment.
