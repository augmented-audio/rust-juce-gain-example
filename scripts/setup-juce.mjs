#!/usr/bin/env zx

await within(async () => {
    await $`mkdir -p ./target/cmake-build-debug`;
    await cd('./target/cmake-build-debug');
    await $`cmake ../..`;
});
