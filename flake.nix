{
  description = "Nix devShell Flake for developing Android app";
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";
    utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, utils, ... } @ inputs:
    utils.lib.eachDefaultSystem (system:
      let
        pkgs = (import nixpkgs {
          inherit system;
          config.android_sdk.accept_license = true;
          config.allowUnfree = true;
        });

        buildToolsVersion = "32.0.0";
        platformToolsVersion = "33.0.2";
        cmakeVersion = "3.22.1";

        # https://github.com/NixOS/nixpkgs/blob/master/pkgs/development/mobile/androidenv/compose-android-packages.nix
        androidComposition = pkgs.androidenv.composeAndroidPackages {
          inherit platformToolsVersion;
          buildToolsVersions = [ buildToolsVersion ];
          platformVersions = [ "27" "32" ];
          cmakeVersions = [ cmakeVersion ];
          includeNDK = true;
          ndkVersion = "24.0.8215888";
        };
        jdk = pkgs.jdk;

        dev-deps = with pkgs; [
          kotlin-language-server
          clang-tools
          rnix-lsp

          gradle
          androidComposition.androidsdk
        ];
      in
      {
        packages = {
          inherit androidComposition;
        };

        devShell = pkgs.mkShell rec {
          name = "r2n-devshell";
          buildInputs = dev-deps;
          ANDROID_SDK_ROOT = "${androidComposition.androidsdk}/libexec/android-sdk";
          ANDROID_NDK_ROOT = "${ANDROID_SDK_ROOT}/ndk-bundle";
          CMAKE_DIR = "${ANDROID_SDK_ROOT}/cmake/${cmakeVersion}";
          JAVA_HOME = jdk.home;
          GRADLE_OPTS = "-Dorg.gradle.project.android.aapt2FromMavenOverride=${ANDROID_SDK_ROOT}/build-tools/${buildToolsVersion}/aapt2";
        };
      });
}

