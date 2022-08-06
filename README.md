# LUST
`LUST` (Lookup Special Tables) is a C++ library of special functions implemented using `FunC` lookup tables.

This library provides LUTs for long double, double, and float for each of its special functions. These tables are built with a relative pointwise error tolerance tol = std::numeric\_limits<T>::epsilon (where T\in{float, double, long double}).

`FunC` can only build LUTs over finite unions of compact intervals. So, `LUST` must use some trickery to build a LUT over unbounded sets.

- eg, take $f:\mathbb{R}\to\mathbb{R} s.t. f(x) = x$. If we have a LUT $l$ for $g(x)=f(tan(\pi x/2))$ over $[-1,1]$ then $l(2\*\arctan(x)/\pi)\approx f(x)$ over $\mathbb{R}$. We'll have to
be careful if $f(\pm\infty)=\pm\infty$, but it seems okay otherwise

See website for a listing of every function we support.


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



