# tumorsphere++

An experimental C++ port of [`tumorsphere_culture`](https://github.com/JeroFotinos/tumorsphere_culture/).

### Requirements

- C++23 compiler
- HDF5 C++ bindings
- Meson
- Ninja

### Building

To build in `builddir`:
- `meson setup -Dbuildtype=release builddir`
- `meson compile -C builddir`
