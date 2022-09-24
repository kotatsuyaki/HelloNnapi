# HelloNnapi

Demo project showing how to use NNAPI with Kotlin and C++.
The code in [`app/src/main/cpp/hellonnapi.cpp`](./app/src/main/cpp/hellonnapi.cpp)
demonstrates the bare minimum required to add two numbers up using
[`ANEURALNETWORKS_ADD`](https://developer.android.com/ndk/reference/group/neural-networks#group___neural_networks_1ggaabbe492c60331b13038e39d4207940e0ad681988001e5f8ab73230a311f4ab034).

## Building with Nix from CLI

In addition to building with Android Studio, this project includes a `flake.nix` file containing a
`devShell` that allows building from the command line.

```sh
# Drop into devShell
$ nix develop '.#'

# Configure paths
$ echo -e "sdk.dir=$ANDROID_SDK_ROOT\nndk.dir=$ANDROID_NDK_ROOT\ncmake.dir=$CMAKE_DIR" > local.properties

# Build using Gradle
# Output is in app/build/outputs/apk/release/
$ gradlew assembleRelease
```
