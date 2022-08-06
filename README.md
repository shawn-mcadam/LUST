# LUST
`LUST` (Lookup Special Tables) is a C++ library of special functions implemented using `FunC` lookup tables.

This library provides LUTs for long double, double, and float for each of its special functions. These tables are built with a relative pointwise error tolerance `tol = std::numeric_limits<T>::epsilon` (where $T\in{float, double, long double}$).

See _website_ for a listing of every special function we support.

## Requirements to use:
- C++11 compliant compiler
- FunC*

* None of `FunC`'s optional dependencies are required

## Requirements to develop LUST:
- C++11 compliant compiler
- FunC*
- Boost
- Armadillo

## Build:
- CMake version >= 3.1

```
mkdir build && cd build/
cmake -DCMAKE_INSTALL_PREFIX=<install-dir>  ..
make install
```

After make install, linking to the library (outside of cmake build) requires:

- `<install-dir>/lib` is in your `LD_LIBRARY_PATH` environment variable,
- `<install-dir>/include/lust` is in your include flags, and
- `-llust` is one of your linking flags

## Implementation notes:

`FunC` can only build LUTs over finite unions of compact intervals. So, `LUST` must use some trickery to build a LUT over unbounded sets.

- eg, if $f:\mathbb{R}\to\mathbb{R}$ and we build a LUT $l$ for $g(x)=f(tan(\pi x/2))$ over $[-1,1]$ then $l(2\*\arctan(x)/\pi)\approx f(x)$ for any $x\in\mathbb{R}$.

