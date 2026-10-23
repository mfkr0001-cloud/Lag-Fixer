Lag Reducer — Geode v1.3.0
Experimental Geometry Dash / Geode mod aimed at reducing stutters on effect-heavy levels. This build extends the particle budget optimization with optional low-graphics switches.
What changed in v1.3.0
Added Low-graphics mode, Disable glow, and Hide game particles switches.
The glow and particle switches use Geometry Dash's existing game variables and are applied when the main menu/level is created, as well as when the settings are changed. If a visual change does not take effect immediately in a running level, restart that level.
Saved the game's initial glow/particle variables so turning off the low-graphics mode can restore the values detected before this mod first changed them.
Strength 10 now retains about 1% of each supported Cocos2d emitter's original particle budget (with at least one slot for small emitters), and reduces the emission rate by the same fraction.
Fixed initialization handling so an emitter that initially reports zero capacity is not permanently cached as a zero-capacity emitter.
Keeps the Lag Reducer shortcut in the pause menu. Does not hook player physics, collision handling, hitboxes, or level timing.
Why some freezes can remain
Particle reduction cannot guarantee zero stutters. A level may be limited by object count, shader/trigger effects, other mods, CPU/GPU limits, memory pressure, or device thermal throttling. Low-graphics mode should reduce some visual workloads, but the result must be tested on the target device and level.
Suggested test
In Geode settings, enable Low-graphics mode, Disable glow, and Hide game particles.
Set Particle optimization strength to 10.
Restart the level and test the same section of Verity.
If it looks too empty, turn off Hide game particles first, then lower the strength to 7–8.
Build with GitHub Actions
Upload/commit the project contents to the `Lag-Fixer` repository, preserving `.github/workflows/android.yml` and the existing folder structure.
Open Actions and run Build Lag Reducer (Android).
From a successful run, download the `LagReducer-Android64` artifact containing the `.geode` file.
This ZIP contains source code, not a precompiled `.geode` file. It has not been compiled or tested inside Geometry Dash in this environment; validate it with GitHub Actions before installing.
