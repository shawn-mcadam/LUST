# lookup-special-tables
A library of special functions implemented using lookup tables.

Thoughts:
- Ideally users will just say:
```c++
easylut<a,b,tol,table_type,safe?> lut = function1 * function2 + ...;
lut(x);
```
to get a LUT table\_type over $[a,b]$ with at most tol pointwise error.

- singleton class for given parameters?
- Is it possible for the LUT to only built when operator() is called for the first time? That way we can try to make a LUT out of the whole expression
- Maybe it's better to make the function a template type?
- Error, bounds, and table type all template parameters?
- If a,b are double then we'll need C++20's double templates




