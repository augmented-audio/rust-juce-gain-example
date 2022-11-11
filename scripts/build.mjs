#!/usr/bin/env zx

const listIdentities = () => {
    $`security find-identity -v -p codesigning`;
}

const buildRust = async () => {
    await $`cargo build --target x86_64-apple-darwin`
    await $`cargo build --target aarch64-apple-darwin`
    const libName = 'libgain_plugin.a';
    await $`lipo -create ./target/x86_64-apple-darwin/debug/${libName}  ./target/aarch64-apple-darwin/debug/${libName} -output ./target/debug/${libName}`

    await $`cargo build --target=aarch64-apple-ios-sim`
    await $`cargo build --target=aarch64-apple-ios`

    // let simulatorLib = `./target/aarch64-apple-ios-sim/debug/${libName}`;
    // const stubSimulatorName = `./target/aarch64-apple-ios-sim/debug/simulator.a`;
    // $`mv ${simulatorLib} ${stubSimulatorName}`
    // await $`lipo -create ${stubSimulatorName} ./target/aarch64-apple-darwin/debug/${libName} -output ./target/debug/${libName}`
}

const buildMain = async () => {
    await $`cmake -Btarget/cmake-build-debug -D"CMAKE_OSX_ARCHITECTURES=arm64;x86_64"`;
    await $`cmake --build ./target/cmake-build-debug`;
}

const buildIos = async () => {
    const iosTargetDir = 'target/cmake-build-ios-debug';
    const target = 'GainPlugin';

    const CODE_SIGN_IDENTITY = process.env.CODE_SIGN_IDENTITY;
    await $`cmake -B${iosTargetDir} -GXcode -DCMAKE_SYSTEM_NAME=iOS -DCMAKE_OSX_DEPLOYMENT_TARGET=11.0 -DCMAKE_XCODE_ATTRIBUTE_DEVELOPMENT_TEAM=${CODE_SIGN_IDENTITY}`;
    await $`cmake --build ${iosTargetDir} --target ${target} -- -sdk iphonesimulator`;

    // await $`xcodebuild -project ${iosTargetDir}/gain_plugin.xcodeproj -scheme GainPlugin_Standalone`
}

await buildRust();
await Promise.all([
    // buildMain(),
    buildIos(),
])