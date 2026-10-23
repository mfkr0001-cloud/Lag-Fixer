Changelog
v1.2.5
Fixed particle-reduction strength ordering so higher values skip more draw calls.
Kept the master enable/disable switch and per-emitter phase offset.
Clarified that rendering reduction cannot guarantee elimination of frame hitches or apparent player jumps.
v1.2.4
Added a per-emitter starting phase to make particle systems less likely to skip on the same frame.
Kept the optimization limited to particle-system drawing.
