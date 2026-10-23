# Changelog

## v1.2.0
- Add a configurable particle-system draw-call reduction level (off through skipping one of every four draw calls).
- Store per-particle-system state using Geode's `Fields` mechanism rather than adding an ordinary data member to a `$modify` class.
- Keep particle updates, gameplay physics, hitboxes, and level timing untouched.
- Retain Android64 GitHub Actions build workflow.
- Build status remains unverified until the SDK-backed CI build succeeds.
