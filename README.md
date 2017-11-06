## Table of contents

&nbsp;[Introduction](https://github.com/mtrebi/ImagesPrimitives#introduction)  <br/> 
&nbsp;[Build](https://github.com/mtrebi/ImagesPrimitives#build)  <br/>
&nbsp;[Execution](https://github.com/mtrebi/ImagesPrimitives#execution)  <br/>
&nbsp;[How it works](https://github.com/mtrebi/ImagesPrimitives#how-it-works)  <br/>
&nbsp;[Gallery](https://github.com/mtrebi/ImagesPrimitives#gallery)  <br/>
&nbsp;[Notes](https://github.com/mtrebi/ImagesPrimitives#notes)  <br/>

----------

## Introduction


## Build

This project has been developed under Windows and using VS 2017. However, no project files are included in the repository. Thus, anyone can build the project using CMake and different generators. 

In order to generate project files for Visual Studio:

```c
// VS 2017
git clone git@github.com:mtrebi/ImagesPrimitives.git
cd ImagesPrimitives
mkdir build
cd build/
cmake .. "Visual Studio 15 2017 Win64"
```

To build the project for any other IDE, just look for the specific [CMake generator keyword.](https://cmake.org/cmake/help/v3.0/manual/cmake-generators.7.html). For Linux, the build instructions differ slightly:

```c
// VS 2017
git clone git@github.com:mtrebi/ImagesPrimitives.git
cd ImagesPrimitives
mkdir build
cd build/
cmake ..
make
```

Then, from Visual Studio you can edit and execute the project. Make sure that __main project is set up as the startup project__

## Execution

After building the executable you can execute it using the following arguments.

| Flag | Default | Description |
| --- | --- | --- |
| `i` | n/a | Directory of target image in bmp format |
| `o` | n/a | Directory of output image in bmp format |
| `g` | 0 | Outputs a sequence of images to create a Gif |
| `t` | 4 | Number of threads to be used |
| `s` | 1000 | Number of random states generated |
| `c` | 100 | Number of Hill Climb steps |
| `m` | 1000 | Maximum number of Hill Climb evaluations. |
| `s` | 1024 | output image size |
| `r` | 16 | Maximum random used for generations/mutations |
| `a` | 128 | Alpha value used for geometric primitives |
| `n` | 250 | Number of geometric primitives used |
| `p` | 0 | Geometric primitive used. 0 - Triangles, 1 - Ellipses |

## How it works

## Gallery

Example images generated with my code:

![Monalisa generated using 500 triangles](https://raw.githubusercontent.com/mtrebi/ImagesPrimitives/master/docs/images/monalisa_500t.bmp)

![Dog generated using 500 triangles](https://raw.githubusercontent.com/mtrebi/ImagesPrimitives/master/docs/images/sona_500t.bmp)

![Colorful Lion generated using 1000 ellipses](https://raw.githubusercontent.com/mtrebi/ImagesPrimitives/master/docs/images/lion_small_1000e.bmp)

![Picture of Girona generated using 4000 ellipses](https://raw.githubusercontent.com/mtrebi/ImagesPrimitives/master/docs/images/girona_4000e.bmp)

## Notes
- This project is based on [primitive](https://github.com/fogleman/primitive), a Go library created by Michael Fogleman.
- It is also based on [Geometrize Haxe](https://github.com/Tw1ddle/geometrize-haxe), a Haxe library created by Sam Twidale
- This project has far less features than the previous libraries. This is because goal of this project was to generate an image, print it on a canvas and give it to my girlfriend as a present for her birthay.

[Picture of the canvas for my gf!]
