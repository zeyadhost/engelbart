# Release Checklist

Use this checklist before creating a public release. Do not create a tag or GitHub release until all required checks pass.

## Required Checks

- [ ] Confirm `git status` is clean.
- [ ] Configure the host build: `cmake -S . -B build`.
- [ ] Build host tests and examples: `cmake --build build`.
- [ ] Run host tests: `ctest --test-dir build --output-on-failure`.
- [ ] Configure the Pico smoke build: `cmake -S examples/pico_tinyusb_smoke -B build-pico-smoke -DPICO_BOARD=pico_w`.
- [ ] Build the Pico smoke firmware: `cmake --build build-pico-smoke`.
- [ ] Confirm the Pico smoke UF2 exists at `build-pico-smoke/engelbart_pico_tinyusb_smoke.uf2`.
- [ ] Check that `README.md` matches the current supported scope.
- [ ] Check that `LICENSE` exists and README links to it.
- [ ] Check that `CHANGELOG.md` has release notes for the version being tagged.

## Optional Hardware Check

- [ ] Put the Pico WH in BOOTSEL mode or use the documented picotool forced reboot path.
- [ ] Flash `build-pico-smoke/engelbart_pico_tinyusb_smoke.uf2`.
- [ ] Confirm the smoke firmware enumerates as `1209:0003`.
- [ ] Confirm `picotool reboot -u -f --vid 0x1209 --pid 0x0003` can return the board to BOOTSEL in the local environment.

## Release Steps

- [ ] Create an annotated git tag for the release.
- [ ] Create the GitHub release from that tag.
- [ ] Verify the release page shows the intended notes.
- [ ] Verify the release does not imply support for deferred features.
