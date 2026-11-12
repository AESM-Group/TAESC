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

Clone with `git clone --recursive`. LibRPA, LibRI, LibComm and LibDDLA are submodules under `src/`, and cereal is in `thirdparty/cereal-d81e2f7`. LibRPA uses LibRI, LibComm and cereal from this tree. greenX and ELPA are downloaded by LibRPA at configure time and cached in `thirdparty/archives/` (not tracked). For offline builds run `scripts/prepare_thirdparty.sh` on a connected machine, copy `thirdparty/archives/` to the build host and configure with `-DLIBRPA_THIRDPARTY_DOWNLOAD=OFF`. CMake 3.18 or newer is required.
