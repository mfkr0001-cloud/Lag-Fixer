Changelog
v1.2.9
Increased high-strength particle reduction: strength 10 now keeps about 3% of each supported emitter's original budget (previously 20%).
Strength also lowers the emission rate to match the particle budget.
Kept the existing safe capacity-shrink behavior to avoid shrinking storage while more particles are alive than the target.
This targets Cocos2d particle systems only; it cannot guarantee elimination of all FPS drops caused by object count, glow, shaders, device load, or other mods.
v1.2.8
Replaced per-frame particle draw skipping with emitter budget/rate reduction.
Added a Lag Reducer shortcut to the pause menu.
