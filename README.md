# GPR

`GPR` is a multi-tool C library project, open source and free to use. The goal
is to offer common functionnalities such as date manipulation, network
management and log writing. It gathers diversified features in a single,
easy to use, library. The targeted compilers are [GCC](https://gcc.gnu.org/)
and [CLANG](https://clang.llvm.org/). The targeted OS are
[Linux](https://www.linux.org) and [MacOS](https://www.apple.com/). This project
involves [GNU](https://www.gnu.org/) extensions.

## Content

- `extras`: Extra features for development
- `include`: Header files
- `models`: Models for development
- `src`: Source files
- `tests`: Unitary tests

## Build & Installation

Required package(s):
- [cmake](https://cmake.org/) : Cross-platform build process tool

Build & install API:
- mkdir build
- cd build
- cmake ..
- make
- make install

Build & install API *(Debug)*:
- mkdir build
- cd build
- cmake -DCMAKE_BUILD_TYPE=Debug ..
- make
- make install

## API Usage

You can include a GPR module in your source code as follows:
```C
#include <GPR/module.h>
```

Make sure to link GPR library at compile time with:
- `-lGPR` for the shared version
- `-lGPR_static` for the static version

## Generated Makefile Commands

- Use the `make (all) (GPR) (GPR_static)` utility to build API
- Use the `make clean` utility to clean cmake API build
- Use the `make cleandoc` utility to clean documentation
- Use the `make cleangraph` utility to clean API graph
- Use the `make clear` utility to clean/cleandoc/cleangraph
- Use the `make doc` utility to generate API documentation ([doxygen](https://www.doxygen.nl/index.html)/[graphviz](https://graphviz.org/) package required)
- Use the `make graph` utility to graph API ([python3](https://www.python.org/)/[graphviz](https://graphviz.org/) package required)
- Use the `make help` to get Makefile possible targets list
- Use the `make indent` utility to auto indent API files ([clang-format](https://clang.llvm.org/) package required)
- Use the `make install` utility to install API
- Use the `make uninstall` utility to uninstall API *(Unix only)*

## Development

Required/recommended package(s):
- [clang-format](https://clang.llvm.org/) >= 13.0.0 : Auto indent C/C++/Objective-C code tool
- [doxygen](https://www.doxygen.nl/index.html) >= 1.9.0 : Auto documentation generation tool
- [graphviz](https://graphviz.org/) >= 2.44.1 : Set of graph drawing tools
- [python3](https://www.python.org/) >= 3.6 : High-level programming language
- [valgrind](https://valgrind.org/) : Memory debugging tool

Add/modify a module:
- Create a branch `F_xxx` for features *(derived from develop)* or `HF_xxx` for hotfixes *(derived from master)*
- Read GUIDELINE file
- Refer to the `models` section if needed
- Add/enhance `include` and `src`
- Add/enhance `test` section with new features, test retrocompatibility and memory leaks
- Auto indent with `make indent`
- Check updating documentation with `make doc`
- Write changes in CHANGELOG for future version

Improve a module:
- Check `// TODO` or `// WARN` markers regularly

## Authors

See the AUTHORS file in this source distribution for more information.  
See also CREDITS for the sources from which this project is inspired.

## Licence

`GPR` is released under BSD-3 clause licence.  
See the LICENCE file in this source distribution for more information.
