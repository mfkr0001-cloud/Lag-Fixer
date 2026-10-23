Changelog
v1.2.4
Stagger particle-system draw skipping across systems so they are less likely to disappear on the same frame.
Keep reduction level 0 as the default; this remains a visual rendering workaround, not a guaranteed FPS fix.
Align metadata with the SDK version used by the successful CI build (Geode SDK 5.11.2).
v1.2.3
Add a custom `logo.png` icon for the mod.
Add a master toggle for particle draw reduction.
Add level 4, skipping one out of every five particle draw calls.
Keep reduction level 0 as the default so installing the mod does not intentionally change rendering.
Retain the attempted Clang 21 `fmt` workaround; the Android build still needs CI verification.
v1.2.2
Add a workaround attempt for Clang 21 consteval errors in the `fmt` dependency during Android builds.
v1.2.0
Add configurable particle-system draw-call reduction levels.
Keep particle updates, gameplay physics, hitboxes, and level timing untouched.
Add Android64 GitHub Actions build workflow.
