# PixelProject-v3

A reimagined version of PixelProject, designed to showcase the latest advancements in my C++ skills and creativity. This project is an opportunity for me to take what worked well in the original PixelProject and build upon it, while also incorporating new ideas and improvements. There have been a number of variants of PixelProject since my first attempt https://github.com/SimplyJpk/PixelProject with that one (at current time of writting) being the most stable even if it is not the most advanced.

This project utilizes a combination of cmake and CPM to manage dependencies, I personally use vscode for development, but it should be possible to use any IDE that supports cmake however I have not gone so far as to confirm this yet, my cmake skills are also vague at best, so the CMakeLists may change as I learn more.

## Plans

PixelProject-v3 doesn't have much in the way of an attack plan, but I do have a few ideas that I would like to implement.

- 64bit sand
    - Problem with v1 and v2 was that the data I could store in sand was limited, I hope to fix this by x2 the chunk texture, and casting out ulong longs instead of uints.
    - Requires additional shader work as still limited by 32bit texture data, but we can get around this by manipulating texels in the shader to offset pixels, if the bandwidth is not a problem, we could potentially store even more data in sand.
- 100% functional chunks, no chunk bleeding. Currently had mixed results, but often times sand has teleported between chunks when not intended.
- Only update chunks that are visible in camera frustum (Already mostly done)
- Only update chunks that are in a dirty state.

## Build

To build this project, you will need to have cmake and CPM installed.
- `cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=D:/vcpkg/scripts/buildsystems/vcpkg.cmake` in the root directory of the project to generate a build directory
- `cmake --build build` in the root directory of the project to build the project which will contain the executable

## Running

The executable will be in the build directory, you can run it by executing the executable in the build directory.

 ## Libraries
 
 I'll update this eventually should the project get far enough along that I think it matters.
 
 ## Contributing
Issues stating problems or feature requests are welcome, as are pull requests and any code contributions.
