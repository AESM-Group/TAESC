# TAESC project

`TAESC` stands for **T**oolkit for **A**dvanced **E**lectronic **S**tructure **C**alculations.

## Directory structure

```
./
|-- binding/
|-- docs/
|-- include/
|-- src/
|-- thirdparty/
|-- CMakeLists.txt
`-- README.md
```

Obtained with `tree --dirsfirst --charset=ascii -F`.

## Dependencies

LibRPA downloads greenX, ELPA and LibDDLA at configure time. For offline builds run `scripts/prepare_thirdparty.sh` on a connected machine, copy `thirdparty/archives/` to the build host and configure with `-DLIBRPA_THIRDPARTY_DOWNLOAD=OFF`. CMake 3.18 or newer is required.
