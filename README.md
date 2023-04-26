# CSE331-DSA-Project

This is a Qt project that showcases the use of various GUI components and the event-driven architecture provided by the Qt framework.

![test xml 13_02_2023 08_13_46 PM](https://user-images.githubusercontent.com/74428638/218540346-45109288-8916-4d6f-b573-959d6e482b0b.png)

## Prerequisites

Before building and running this project, you will need to have the following installed on your system:

- Qt 7.0.0
- A C++ compiler (e.g. GCC, MSVC, or Clang)
- Qt Creator or any IDE you like (Make sure to check the comments in the root ``CMakeLists.txt`` file for any outdated configurations such as Qt version)
- CMake 3.23.1

## Features

- Compressing and Decompressing files
- Validating XML files for errors and try a way for fixing them
- Minifying and Preitifying XML files
- Converting XML files to JSON format
- Representing XML files using GRAPH data structure (SNA)

## Building the project

Here are the steps to build and run the project (for windows only, we didn't run the project on any other platform):

1. If you don't have a compiler, follow the steps in the link below to install ``mingw64`` from ``msys2``. I'm using ``GNU 11.2.0``. Note that some old versions such as ``GNU 6.0.0`` won't generate ``Makefile`` file correctly.
```
https://www.msys2.org/
```
2. Download and install qt creator
3. Clone the repository
```bash
git clone https://github.com/Mohammed84Farouk/CSE331-DSA-Project.git
```
3. If you are using Qt Creator: select ``File -> Open File or Project`` or on keyboard ``Ctrl+o``
4. Select root ``CMakeLists.txt`` file
5. Press configure project button on the right
6. Run the project
7. If you are not using Qt Creator: have a look at the root ``CMakeLists.txt`` file and uncomment the lines needed.
8. Create a build directory for CMake output
```bash
mkdir .build
```
9. Go inside
```bash
cd .build
```
10. Produce the ``Makefile`` file needed
```bash
cmake .. -G "MinGW Makefiles"
```
11. Build the project (if that doesn't work for you, just find out the correct naming of make excutable, for me it's ``mingw32-make``)
```bash
make
```
12. Run the project
```bash
./xmlparser.exe
```
13. Any update you make in the source files just repeat steps 10 - 12

## Acknowledgements

We would like to extend our gratitude to the Qt community for their support and for creating such a powerful framework for developing GUI applications. We would also like to acknowledge the open-source projects and libraries that were used in the development of this project.

## About US

We are a group of five students currently enrolled at Ain Shams University, Faculty of Engineering, who share a passion for solving complex problems and creating innovative solutions. Our team consists of a five Computer & Systems Engineering majors, providing a unique blend of skills and perspectives.

At the heart of our team is a shared commitment to hard work, collaboration, and making a difference. Whether we're working on a school project or a startup, we approach every challenge with enthusiasm, creativity, and a drive to succeed.

We are excited to see what the future holds and are committed to making the most of every opportunity that comes our way.

- Mohammed Mohammed Farouk ``1901472``
- Mostafa Mohamed Ahmed Abdelaal ``1803093``
- Ahmed Reda Ibrahim ``1901164``
- Mostafa Monged Mostafa Mohamed ``1900563``
- Saif Salah Eldeen Yahya Mostafa ``1901529``
