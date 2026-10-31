# Sad Library

A minimalist C99 base library for systems, graphics and application programming.

## Features

### Data Structures ([Link](./include/sadcontainers.h))

Generic, efficient containers:

- `sad_array`: Dynamic growing array.
- `sad_string`: Dynamic string. Supports conversion to utf-8, utf-16 and utf-32. Meant to be used in tandem with `string.h`.
- `sad_fmap`: Array based map using binary search algorithm.
- `sad_fdict`: Dynamic open addressing hashtable/dictionary using robin hood hashing.
- `sad_sdict`: Open addressing sparse hashtable/dictionary using robin hood hashing.

### Linear Algebra ([Link](./include/sadmath.h))

Math types optimized for 3D graphics:

- Vectors: `sad_vec2`, `sad_vec3`, `sad_vec4`.
- Quaternion: `sad_quat` for smooth rotations.
- Matrices: `sad_mat3`, `sad_mat4` (column-major), for transformations.

## How to build

### Windows

While in project root:

1. run `third-party/premake/premake5.exe vs2022` in terminal
2. open `sadlib.sln` in Visual Studio
3. select config and platform
4. build
5. find `.dll` and `.lib` file in `bin/{config}`

### Linux (untested)

While in project root:

1. run `third-party/premake/premake5 gmake2` in terminal
2. run `make config="release_linux64"` in Visual Studio
3. select config and platform
4. build
5. find `.so` and `.a` file in `bin/{config}`

### Single or Double Precision

Define `SAD_FLOAT_64` to make `sad_float` a double. ⚠️ SadLib has to be compiled using the same settings as the application when used as dll/shared library. ⚠️

## License

[MIT License](https://github.com/Schmicki/SadLib/blob/main/LICENSE)
