Changelog
v1.2.6
Expanded optimization strength from 0–4 to 0–10.
Added particle update batching with accumulated elapsed time for higher strength settings, in addition to drawing reduction.
Kept gameplay physics, collisions, hitboxes, and level timing outside the hooks.
Documented that high settings can make particles choppy and do not guarantee elimination of unrelated frame hitches.
v1.2.5
Fixed particle-reduction strength ordering so higher values skip more draw calls.
Kept the master enable/disable switch and per-emitter phase offset.
v1.2.4
Added a per-emitter starting phase to make particle systems less likely to skip on the same frame.
