# [OpenMPT](https://openmpt.org) for Godot

These are bindings for [libopenmpt](https://github.com/OpenMPT/openmpt/tree/master/libopenmpt) (The library version of the playback engine of [OpenMPT](https://openmpt.org), the modtracker software) made for Godot which allows you to play almost any tracker module format (like .xm or .it or .s3m or even something like .umx aka the Unreal Engine 1 Music format) under the sun as a normal AudioStream (as if it was just a WAV or OGG file)

## Why?

Because my game uses mod-tracker music inspired by Deus-Ex, and it also makes looping and dynamic music easier (haven't implemented channel muting and stuff yet but it should be trivial to implement)

## Where can I find a pre-built addon?

You can't right now, this has to be built from source because I haven't setup any way for there to be proper releases yet or anything. It is experimental of course.

## What platforms does it support?

In theory, all of them. In practice building for certain platforms sucks and so hasn't been tested. I have only tested Windows and Linux thus far. As for MacOS... I don't have a Mac neither do I have the money for one so unless someone else would like to volunteer to compile it and provide fixes if anything goes wrong on Mac, I can't test it myself. Blame Apple for having stupid licensing in regards to their system headers, otherwise cross-compilation probably wouldn't be that hard.

As for mobile platforms, I haven't tested them either because I don't make mobile games and iOS has a lot of the same issues as MacOS does (don't have an iPhone either). Android should probably work, but it hasn't been tested. (Just like with MacOS open an issue if you run into any problems)

## How to build

This project uses CMake so it's recommended to use that (even for building godot-cpp as the DLL names are different and I had issues mixing SCons and CMake before).

It has been tested on Windows and Linux (via Ubuntu WSL on Clang). MacOS might work but you're on your own (at least for now), open an issue if it has any problems and I'll try to address them or you can address them yourself in a PR (probably easier if you can, since I can't test things without a Mac).

I recommend using Visual Studio 2022 and its CMake integration.

## Windows Build Instructions

### godot-cpp

Open the `godot-cpp` subdirectory in Visual Studio 2022 and build the project in the `x64-Debug` default configuration. (May take a while)

Now create a configuration for `x64-Release` and build it in that too (you may have to do a full rebuild). (May take a little longer)

Now in the `out/build` folder you should have two directories for each config.

Go into the `bin` folder of each and copy the DLLs to a new `bin` folder at the root of the `godot-cpp` subdirectory.

Also copy the `gen` folder from one of the folders into the root of the `godot-cpp` subdirectory.

### The Addon

Open the root repository directory in Visual Studio 2022 and build it in `x64-Debug`.

Create a configuration for `x64-Release` just like before and build that as well.

After this is done you will have both `.dll`s in the `bin` directory of the addon folder.

## Linux Build Instructions

### Requirements

You will need `cmake`, either the `gcc` toolchain or the `clang`/`llvm` toolchain. Also recommended is `ninja-build` for the build system.

### godot-cpp

Open the `godot-cpp` subdirectory in a terminal, run `cmake . -B build`. If you are using ninja add `-G Ninja`, also if you wish to build for debug / the editor add `-DCMAKE_BUILD_TYPE=Debug`
(If you want both Debug and Release you will have to do these instructions two times, one with the Debug flag, and one without).

`cd build`, then run your build system. In this case, `ninja`.

Copy the `.so` file(s) from the `bin` directory of the `build` subdirectory to a new `bin` directory at the root of the `godot-cpp` directory.

Also copy the `gen` directory to the root of the `godot-cpp` directory.

### The Addon

Now change to the root repository directory. Run cmake just like before, in this case `cmake . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug`,
then `cd build && ninja`.

After this is done you will have a `.so` file(s) in the `bin` directory of the addon folder.

## After Build

Now you're pretty much done, just copy the addon directory to your Godot project, reload it if you have it open, enable the importer plugin and boom.
You now can play your mod-tracker formats as a regular audio stream (and even manipulate the playback of them a little too).

For more information on what functions you can use either look at the source code, or look at the in-editor docs for the types `AudioStreamMPT` and `AudioStreamPlaybackMPT`.

## Problems?

Open an issue, I'll be happy to take a look. I can't guarantee support though, I'm just a solo hobby developer, but I'm more than willing to take feedback or try to help if I can.

# This code is experimental, however it is functional (at least on my machine)
