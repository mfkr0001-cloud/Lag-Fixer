Changelog
v1.3.0
Added low-graphics mode with separate glow and game-particle switches.
Captures original glow/particle game-variable values once and uses them as the restore baseline when low-graphics mode is turned off.
Strength 10 now keeps about 1% of supported Cocos2d particle emitter budgets.
Fixed lazy initialization when a particle system initially reports no capacity.
Reapplies visual settings on menu/level creation and after changing a visual setting.
v1.2.9
Increased high-strength particle reduction: strength 10 retained about 3% of each supported emitter's original budget.
Strength also lowered the emission rate.
Retained safe capacity-shrink behaviour.
v1.2.8
Replaced per-frame particle draw skipping with emitter budget/rate reduction.
Added a Lag Reducer shortcut to the pause menu.
