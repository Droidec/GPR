# GHANGELOG

This CHANGELOG indicates the different versions of the `GPR` project.

## Version 0.0.23

- Add C standard in `cmake` compilation options
- Update `Doxygen` rendering
- Change headers inclusion order
- Update CREDITS
- Fix typo

## Version 0.0.22

- Add the `color` command-line option to grapher
- Fix misuse types in `gpr_str` module
- Fix typo

## Version 0.0.21

- Introduce `gpr_bit` module for bit management
- Introduce `gpr_str` module for C-string management
- Introduce `gpr_tree` module for n-ary tree management
- Add GPR_ERR_LOOP_DETECTED to `gpr_err` module
- Increase log messages maximum size in `gpr_log` module
- Change some variable names to reduce their size
- Update GUIDELINE
- Update README
- Remove TODO file
- Fix typo

## Version 0.0.20

- Introduce `gpr_arrtpl` module for templated dynamic array management
- Introduce `gpr_bin` module for binary manipulation
- Introduce `gpr_buffer` module for data array management
- Rename `gpr_builtin` module to `gpr_utils`
- Update clang-format style
- Update Doxyfile configuration
- Update GUIDELINE
- Update README

## Version 0.0.19

- Rename HOW-TO-DEV to GUIDELINE and rewrite it in Markdown
- Rewrite CHANGELOG in Markdown
- Rename `gpr_network` module to `gpr_net`
- Add optimization flag `O3` in Release version
- Fix Debug version
- Improve `gpr_klist` module

## Version 0.0.18

- Add compilation flags
- Update models
- Update development rules
- Introduce `gpr_klist` module for Linux Kernel linked list
- Add macros to `gpr_dlklist` module
- Add macros to `gpr_err` module
- Add CREDITS file

## Version 0.0.17

- Update LICENCE
- Update README (Markdown)
- Add AUTHORS file

## Version 0.0.16

- Try to find external packages for additional Makefile targets
- Compile GPR library in both static/shared
- Introduce `gpr_array` module for dynamic array management

## Version 0.0.15

- Hide clang-format style
- Introduce CMAKE

## Version 0.0.14

- Improve generated documentation

## Version 0.0.13

- Remove revision number from Doxyfile
- Add "replace" function to dlklist module
- Optimize dlklist searching
- Add a DEBUG compile mode

## Version 0.0.12

- Remove doc from package
- Add Makefile targets to clean more specifically

## Version 0.0.11

- Add the `standalone` command-line option to grapher
- Improve `search` functionnality in dlklist module
- Introduce `Doxygen` documentation

## Version 0.0.10

- Improve grapher
- Remove inclusion documentation

## Version 0.0.9

- Introduce home-made grapher in extras section

## Version 0.0.8

- Change Licence to BSD-3 clause licence
- Redesign of general module comments and preambles
- Removal of watchdog in double linked list module

## Version 0.0.7

- Fix clang-format + auto indent source code

## Version 0.0.6

- Introduce `gpr_dlklist` for double linked list management

## Version 0.0.5

- Update Makefile + README

## Version 0.0.4

- Introduce `gpr_network` for network management
- Drop hungarian notation
- Various MacOS fixes

## Version 0.0.3

- Various `Cygwin` fixes

## Version 0.0.2

- Introduce `gpr` for library macros
- Introduce `gpr_builtin` to the GPR library for builtin macros/functions
- Introduce `gpr_err` for error management
- Introduce `gpr_log` for log management

## Version 0.0.1

- Introduce doc, models, and extras stuff
- Introduce `gpr_time` module to the GPR library for time management
