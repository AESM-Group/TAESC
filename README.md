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

Clone with `git clone --recursive`. LibRPA, LibRI, LibComm and LibDDLA are submodules under `src/`. cereal is downloaded by TAESC at configure time (pinned commit, SHA-256 checked) and provided to LibRPA as `cereal::cereal`; pass `-DCEREAL_INCLUDE_DIR=<path>` to use an existing copy. LibRPA uses LibRI and LibComm from this tree. greenX and ELPA are downloaded by LibRPA at configure time. All archives are cached in `thirdparty/archives/` (not tracked). For offline builds run `scripts/prepare_thirdparty.sh` on a connected machine, copy `thirdparty/archives/` to the build host and configure with `-DTAESC_THIRDPARTY_DOWNLOAD=OFF`. CMake 3.18 or newer is required.
