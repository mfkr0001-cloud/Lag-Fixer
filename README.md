Lag Reducer — Geode v1.2.9
Experimental Geometry Dash / Geode mod that reduces the workload of particle systems on effect-heavy levels.
What changed in v1.2.9
Removes frame-by-frame particle draw skipping, which could make particle effects flicker or look choppy.
Settings now scale both particle emission rate and the maximum particle budget. Strength `0` restores the original settings; strengths `1–10` retain about `85%, 72%, 60%, 48%, 36%, 25%, 16%, 10%, 6%, 3%` of each supported emitter's original capacity/rate.
When changing settings mid-level, the new emission rate applies immediately. The maximum particle capacity is reduced only when the live particle count is low enough to avoid shrinking the particle array below the number of active particles.
Keeps the `Lag Reducer` shortcut in the level pause menu to open the normal Geode settings popup.
Does not hook player physics, collision handling, hitboxes, or level timing.
Important limitations
The mod is experimental and cannot guarantee 60 FPS on every level or device. This revision targets particle simulation/emission load rather than skipping visible draw frames. High strengths can substantially reduce visual effects. If stutters remain at strength `10`, the bottleneck may be elsewhere (level triggers, shaders, other mods, device throttling, or frame pacing).
Android build with GitHub Actions
Upload/commit the project contents to your `Lag-Fixer` repository, including `.github/workflows/android.yml` and `logo.png`.
Open Actions and wait for Build Lag Reducer (Android) to finish.
From a successful workflow run, download the `LagReducer-Android64` artifact containing the `.geode` file.
The workflow uses `sdk: nightly`; `mod.json` retains Geode `5.11.2` and Geometry Dash `2.2081` from the currently successful setup.
Verification
This is a source ZIP, not a precompiled `.geode` package. It has not been compiled or tested inside Geometry Dash in this environment. Verify it with GitHub Actions before installing it.

Note: the mod targets Cocos2d particle systems only. Heavy object counts, glow, shaders, device thermal/power limits, and other mods can still cause frame drops.
