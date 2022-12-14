# rust-juce-gain-example
This example passes data from JUCE to Rust to process audio.

Currently, is uses:
* `cbindgen` - but could use `cxx` (https://github.com/dtolnay/cxx)
* CMake / JUCE CMake

View
[`audio-buffers`](https://github.com/augmented-audio/rust-juce-gain-example/compare/audio-buffers)
for a higher-level example using `AudioProcessor` abstractions and `AudioBuffer`.

## Building

* Install node.js
* Install `zx`

```
./scripts/build.mjs
```
