# ray-tracing-personal
-----------------------------------------------------------------------

Profile(windows):

(If your system is win10, you will download the branch called "master")

Base on ray tracing in one weekend/next week

- g++ version: g++ (x86_64-posix-seh-rev0, Built by MinGW-W64 project) 8.1.0

- environment: vscode + win10

- Use the following code to compile generally:

    - **g++ src/main.cpp -o output/t**

- Running:

    - **./output/t**

    - or

    - **./output/t 400 100 20 10**

    - (tips: ./output/t screen_with screen_height samples_per_pixel ray_tracing_depth)

-----------------------------------------------------------------------

Profile(linux):

(If your system is linux, you will download the branch called "linux-master")

- g++ version: g++ (Ubuntu 9.4.0-1ubuntu1~20.04.1) 9.4.0

- environment: vscode + ubuntu20.04

- Use the following code to run generally(The executable program is in output):

    - **make run**

    - clean objs file: **make clean**

-----------------------------------------------------------------------

# v1.0 (2022.04)

1. Base code.

# v1.1 (2023.02)

1. Add a menu that sets screen width, screen height, samples per pixel and ray trancing depth.
2. Add a header file called sstream in common.h
3. Adjust files structure.

# v1.2 (2023.03)

1. Optimize multithreaded.
    - The numbers of threads is twice the numbers of cpu cores.
    - Running time is from 1min34s to 24s, which running parameters is 400 100 20 10.
2. Add some cross platform code.


# v1.3 (2023.03)

1. Add BVH(Bounding Volume Hierarchies).
2. The speed of code has increased by 300% - 2000% (It depends on the number of objects).

# v1.4 (2023.03)

1. Add perlin.
2. Add image texture.
3. Add Rectangle Class.
4. Add Box Class.
5. Build classic scene: Cornell Box.
6. Basically complete v1 version.

# v2.0 (2023.03.16)

1. Add the support of linux.
2. Add process time for linux(Fix the invalid of process time calculation which in windows.)
3. Fix the invalid of sleep function for linux.
4. Rebuild file structure.(Separate .h and .cc and Sort.)
5. Add Makefile

# v2.1 (2023.03.21)

1. Add GUI.
2. Add scene manager.