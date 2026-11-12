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

Clone with `git clone --recursive` and run `git lfs pull`. LibRPA, LibRI, LibComm and LibDDLA are submodules under `src/`, cereal is in `thirdparty/cereal-d81e2f7`, and the pinned greenX and ELPA archives are in `thirdparty/archives/` (Git LFS). LibRPA uses LibRI, LibComm and cereal from this tree, and configuration does not download anything. Use `scripts/prepare_thirdparty.sh` to refresh the archives. CMake 3.18 or newer is required.
