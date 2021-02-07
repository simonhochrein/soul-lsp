# SOUL LSP

Language Server for [SOUL](http://github.com/soul-lang/SOUL)

:warning: This project is in active development :warning:

## Features Planned:

- go to definition
- hover

## Features in Development

- completion (keyword completion is functional, more soon)

## Features Implemented:

- diagnostics

## Setup

```
$ git submodule update --init --recursive
$ mkdir build
$ cd build
$ cmake ..
$ cmake --build .
```

After the project builds, open the vscode_plugin folder in vscode, yarn install, and run the extension.

## Notes

The syntax file and language configuration were copied from the main [SOUL respository](http://github.com/soul-lang/SOUL)
