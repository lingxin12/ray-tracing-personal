# ray-tracing-personal
-----------------------------------------------------------------------

Base on ray tracing in one weekend/next week

g++ version: g++ (x86_64-posix-seh-rev0, Built by MinGW-W64 project) 8.1.0

environment: vscode + win10

Use the following code to compile generally:

g++ test.cpp -o t

Running:

./t

or

./t 400 100 20 10 (./t screen_with screen_height samples_per_pixel ray_tracing_depth)

-----------------------------------------------------------------------

v1.1

1. Add a menu that sets screen width, screen height, samples per pixel and ray trancing depth.
2. Add a header file called sstream in common.h
3. Adjust files structure.