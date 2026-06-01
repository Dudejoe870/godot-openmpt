# [OpenMPT](https://openmpt.org) for Godot

These are bindings for [libopenmpt](https://github.com/OpenMPT/openmpt/tree/master/libopenmpt) (The library version of the playback engine of [OpenMPT](https://openmpt.org), the modtracker software) made for Godot which allows you to play almost any tracker module format (like .xm or .it or .s3m or even something like .umx aka the Unreal Engine 1 Music format) under the sun as a normal AudioStream (as if it was just a WAV or OGG file)

Minimum Godot Version: 4.3

## Why?

Because my game uses mod-tracker music inspired by Deus-Ex, and it also makes looping and dynamic music easier.

## Where can I find a pre-built addon?

Just go to the prebuilt branch and use the download ZIP feature of Github, or use "Releases" on the side, extract it into your project and you're done!

If you want to take a look at a commit that's more recent, then you can download the addon as a CI build artifact from the Github workflow action.

Unfortunately there are no MacOS, Android, iOS, or any 32-bit binaries currently prebuilt. If you need any of those you will have to build them yourself and add them to the .gdextension file.

## Documentation

For information on what functions you can use either look at the source code, or look at the in-editor docs for the types `AudioStreamMPT` and `AudioStreamPlaybackMPT`.

There is no official written documentation for the time being, but just the function names and parameter names should most likely be helpful enough; and if you know any amount of C++ the code should be pretty easy to parse.

## What platforms does it support?

In theory, all of them. In practice we have tested only:

- Windows
- Linux
- Web (in a Linux development environment with Threads Enabled)


## Building godot-cpp

[Godot-cpp](https://github.com/godotengine/godot-cpp) version 10.x is a prerequisite, please follow the project instructions to set up your build environment correctly.

In order to build the web platform, emscripten should be installed on the system.

If godot-cpp builds, then godot-openmpt probably will.

*Make sure you have the godot-cpp project directory as a sibling of godot-openmpt directory.*

Run these in the godot-cpp directory for each platform you intend to support (replace PLATFORM with web, linux, windows, ...):

```
 scons api_version=4.3 target=template_debug platform=PLATFORM
 scons api_version=4.3 target=template_release platform=PLATFORM
```

## Windows Build Instructions

### Requirements

I recommend using Visual Studio 2022 and its CMake integration, but through a Visual Studio Developer Command Prompt; which comes with VS2022.

### The Addon

Run CMake in the root repository directory `cmake . -B build -G Ninja -DCMAKE_BUILD_TYPE=<INSERT EITHER Release OR Debug HERE>`,
then `ninja`.

You can run it again for the opposite configuration if you want both debug and release.

After this is done you will have a `.dll` file(s) in the `bin` directory of the addon folder.

## Linux Build Instructions

### Requirements

You will need `cmake`, either the `gcc` toolchain or the `clang`/`llvm` toolchain (this is actually relatively untested as of the latest buildsystem changes, so it may or may not work with Clang anymore). 
Also recommended is `ninja-build` for the build system.

### The Addon

It's pretty much the same as the Windows instructions:

Run CMake in the root repository directory, `cmake . -B build -G Ninja -DCMAKE_BUILD_TYPE=<INSERT EITHER Release OR Debug HERE>`,
then `ninja`.

You can run it again for the opposite configuration if you want both debug and release.

After this is done you will have a `.so` file(s) in the `bin` directory of the addon folder.

## Web Build Instruction (inside a Linux system)

### Requirements

Emscripten installed on the system.

### The Addon

It's pretty much the same as the Linux instructions, except for -DPLATFORM=web

Run CMake in the root repository directory, `cmake . -B build -G Ninja -DCMAKE_BUILD_TYPE=<INSERT EITHER Release OR Debug HERE> -DPLATFORM=web`,
then `ninja`.

You can run it again for the opposite configuration if you want both debug and release.

After this is done you will have a `.wasm` file(s) in the `bin` directory of the addon folder.

## After Build

Now you're pretty much done, just copy the addon directory to your Godot project, reload it if you have it open, enable the importer plugin and boom.
You now can play your mod-tracker formats as a regular audio stream (and even manipulate the playback of them quite a lot).

## Problems?

Open an issue, I'll be happy to take a look. I can't guarantee support though, I'm just a solo hobby developer, but I'm more than willing to take feedback or try to help if I can.

