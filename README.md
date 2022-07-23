# lookup-special-tables
A library of special functions implemented using lookup tables.

### Thoughts:
The good idea is coming first:
- This library should NOT be header only!!!
- Having users choose table bounds & tolerance is perhaps too cumbersome
- Choose the tol & table type based on the precision used (tol = std::numeric\_limits<T>::epsilon for T\in{float, double, long double})
- We can build a LUT over a functions entire bounds with some (hopefully cheap) transformation to make all the information we need about
that function exist within the interval $[-1,1]$ (or whatever interval is convenient).
- eg, take $f:\mathbb{R}\to\mathbb{R} s.t. f(x) = x$. If we have a LUT $l$ for $g(x)=f(tan(\pi x/2))$ over $[-1,1]$ then $l(2\*\arctan(x)/\pi)\approx f(x)$ over $\mathbb{R}$. We'll have to
be careful if $f(\pm\infty)=\pm\infty$, but it seems okay otherwise
- We just need a reaaaaally fast method of computing tanx (or any other cheaper bijection)
- We can avoid having any dependencies (other than FunC) if we include json files for each supported special function


Other options that _might_ be worth pursuing:
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

