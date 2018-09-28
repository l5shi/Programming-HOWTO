# Function Definition

You can define a Func in multiple passes. [Halide tutorial code](http://halide-lang.org/docs/tutorial_2lesson_09_update_definitions_8cpp-example.html)
- *Pure* definition: the first, *pure* variables 
- *Update/Reduction" definitions: subsequent ones. 

Rules on variables in function definition:
- **Rule on LHS** 
  - Variables **within** () in reference to functions: used variable must first appear at its location in *pure* definition within the *same* function reference.
```c++
// legal updates
f(x  , x + 1) = x + 8;
f(y/2, y)     = f(0, y) * 17;

// Second argument to f on the left-hand-side must be 'y', not 'y + 1'.
f(y, y + 1) = y + 8;
// Arguments to f on the left-hand-side are in the wrong places.
f(y, x) = y - x;
```
- **Rule on RHS**
  - [**Recursive** reference](https://github.com/halide/Halide/issues/297): We imposed the rule that all **recursive** references to a Func on the **RHS** must contain the same **pure** vars in the same positions. This prevents all sorts of problems involving the _schedule changing the meaning_, by preventing all cross-talk between distinct values of the pure vars. That way it's safe to compute, recompute, cache, etc for any arbitrary domain of the pure vars.

  * **Free** variable: (variables **outside** reference to functions): *free* variables on the right-hand-side must appear on the left-side
```c++
// 1st argument to f on the right-hand-side must be 'x', not 'x + 1'.
f(x, 0)     = f(x + 1, 0);

// Free variables appear on the right-hand-side but not the left-hand-side.
f(3, 4) = x + y;
```

## Reduction

[Halide tutorial code](http://halide-lang.org/docs/tutorial_2lesson_13_tuples_8cpp-example.html)
>**Tuple reductions**: tuples are particularly useful in reductions. The simplest example is an argmax. Halide provides built-in reductions: argmax, argmin, sum, product, maximum, and minimum.

 Reductions are [higher order functions](https://hackernoon.com/higher-order-functions-what-are-they-be74111659e8): Functional programming?

 - [Fold or reduce (higher order function)](https://www.wikiwand.com/en/Fold_(higher-order_function))

 - [Map, filter & reduce in python](http://book.pythontips.com/en/latest/map_filter.html): [reduce in python](https://www.geeksforgeeks.org/reduce-in-python/)
   - `reduce`: two arguments in, one result out
   - 