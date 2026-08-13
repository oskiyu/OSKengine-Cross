# OSKengine 
![alt text](./media/card.png?raw=true)

**NOTE: this project is still in early development and will undergo deep changes.**

**OSKengine** is a general-purpose, cross-platform 3D game engine.

Compiled using CMake presets, dependencies managed with vcpkg.

## Main features

- ECS architecture.
- Low-level, API agnostic rendering.
- *Deferred* PBR render systems, with:
    - Cascaded shadow mapping.
    - Compute based post-processing.
    - HDR bloom.
    - 3D animation support.
    - TAA.
    - Tessellation & ray-tracing support.
- Extensive and detailed API documentation (in Spanish).

## Cross-platform support

- Windows (tested under Windows 11).
- Linux (tested under Ubuntu 24.04).
- Android (tested on Android API 34).

Currently using Vulkan as its only low-level graphics API. More platforms and graphics APIs may be supported in the future.

## Repo structure

- `src/`: OSKengine source code.
- `demo/`: minimal example, used for testing.
- `external/`: external dependencies.
- `scripts/`: Python scripts for Android demo installation.
- `media/`: contains the OSKengine logo used in this README.