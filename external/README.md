## Clone Submodules
Clone the external repositories.
```
git submodule update --init
```

## AppCommon
This repository contains common/shared application code, such as xml handlers.
The files in this repository are not built independently, but rather included
into the final build.

## Models
This repository contains the engineering calculation libraries. See the
`external/Models/build` subdirectory for build instructions.

## wxWidgets
This repository contains the GUI framework libraries, which are highly
configurable. Detailed build instructions below.

### Linux
Install the required packages. On Ubuntu:
```
sudo apt-get update
sudo apt-get install libgtk2.0-dev
```

Change to the wxWidgets repository.
```
cd external/wxWidgets
```

#### Debug
Create a build directory.
```
mkdir build-debug
cd build-debug
```

Configure the library build.
```
../configure --disable-shared --enable-debug_gdb --enable-std_string_conv_in_wxstring
```

Build the libraries.
```
make
cd ..
```

#### Release
Create a build directory.
```
mkdir build-release
cd build-release
```

Configure the release library build.
```
../configure --disable-shared --disable-debug --enable-std_string_conv_in_wxstring
```

Build the libraries.
```
make
cd ..
```

### Windows
Open the Visual Studio project solution for the wxWidgets libraries.
```
build/msw/wx_vc14.sln
```

Convert all projects to statically link to the Windows CRT.
```
Project -> Properties -> C++ -> Code Generation
  Debug = MTd
  Release = MT
```

Select the platform.
```
x64
```

Build the following configurations.
```
Debug
Release
```
