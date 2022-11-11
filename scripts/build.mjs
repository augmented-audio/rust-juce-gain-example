#!/usr/bin/env zx

const CODE_SIGN_IDENTITY = process.env.CODE_SIGN_IDENTITY;
const listIdentities = () => {
  $`security find-identity -v -p codesigning`;
};

const buildRust = async () => {
  await $`cargo build --target x86_64-apple-darwin`;
  await $`cargo build --target aarch64-apple-darwin`;
  const libName = "libgain_plugin.a";
  await $`lipo -create ./target/x86_64-apple-darwin/debug/${libName}  ./target/aarch64-apple-darwin/debug/${libName} -output ./target/debug/${libName}`;

  await $`cargo build --target=aarch64-apple-ios-sim`;
  await $`cargo build --target=aarch64-apple-ios`;
};

const buildMain = async () => {
  const defines = [
    '-D"CMAKE_OSX_ARCHITECTURES=arm64;x86_64"',
    `-DCMAKE_XCODE_ATTRIBUTE_DEVELOPMENT_TEAM=${CODE_SIGN_IDENTITY}`,
  ];
  await $`cmake -Btarget/cmake-build-debug ${defines}`;
  await $`cmake --build ./target/cmake-build-debug`;
};

const buildIos = async () => {
  const iosTargetDir = "target/cmake-build-ios-debug";
  const target = "GainPlugin";

  const defines = [
    "-DCMAKE_SYSTEM_NAME=iOS",
    "-DCMAKE_OSX_DEPLOYMENT_TARGET=11.0",
    `-DCMAKE_XCODE_ATTRIBUTE_DEVELOPMENT_TEAM=${CODE_SIGN_IDENTITY}`,
  ];
  await $`cmake -B${iosTargetDir} -GXcode ${defines}`;
  await $`cmake --build ${iosTargetDir} --target ${target} -- -sdk iphonesimulator`;
};

await buildRust();
await buildMain();
await buildIos();
