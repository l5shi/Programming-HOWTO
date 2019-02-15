# Halide Concepts

## Func

A `Func` is a ==handle== on the internal Halide function that 

A halide function represents one stage in a Halide pipeline, and is the unit by which we schedule things. By default, they are **aggressively inlined**, so you are encouraged to make lots of little functions, rather than storing things in Exprs.

```c++
 class Func {
 	Internal::Function func; // A reference-counted handle to Halide's internal representation of
  * a function. Similar to a front-end Func object, but with no
  * syntactic sugar to help with definitions.
```
```c++
//A fragment of front-end syntax of the form f(x, y, z), where x, y, z are Vars or Exprs.

//If could be the left hand side of a definition or an update definition, or it could be a call to a function. We don't know until we see how this object gets used.
class FuncRef {
     Internal::Function func;
```
```c++
class Function {
      FunctionPtr contents;
```
```c++
struct FunctionPtr {
     /** A strong and weak pointer to the group. Only one of these
      * should be non-zero. */
     // @{
     IntrusivePtr<FunctionGroup> strong;
     FunctionGroup *weak = nullptr;
```

```c++
/** Functions are allocated in groups for memory management. Each
  * group has a ref count associated with it. All within-group
  * references must be weak. If there are any references from outside
  * the group, at least one must be strong.  Within-group references
  * may form cycles, but there may not be reference cycles that span
  * multiple groups. These rules are not enforced automatically. */
 struct FunctionGroup;
 
 /** The opaque struct describing a Halide function. Wrap it in a
  * Function object to access it. */
 struct FunctionContents;
```

### Pure/Init and Update Definition

Func **update** definition 

- can have `Expr` and `RDom` on LHS.
- Pure `Var` can only appear unmodified on LHS and at the same location as in *pure* definition.

See MIT slidedeck 4

### Buffer to Func

```c++
Halide::Func::Func(Buffer< T > & im)
```

Construct a new [Func](http://halide-lang.org/docs/class_halide_1_1_func.html) to wrap a [Buffer](http://halide-lang.org/docs/class_halide_1_1_buffer.html). (from Halide doc)

### Func Definition vs. Assignment

Func **definition** with arguments, e.g., `(x,y)`: 
```c++
f(x, y) = x + y;
```

Func variable assignment without argument: ==can only be done once==?

- Can not assign a Func to a Func variable??
```c++
Func f;
f(x,y) = x + y;
Func g;
g = f; // NOT do this??
g(x,y)=f(x,y); // DO this instead
```
- OK to assign a Func returned by a C/C++ function

```c++
Func gu = GaussianUpsample2(
                BoundaryConditions::mirror_interior(bufY, 0, width, 0, height),
                "GaussianUp");

Func GaussianUpsample2(Func img, const char * returnedFuncName)
{
	Var x("x"), y("y");
	Func gu(returnedFuncName);
	Func upX(string(returnedFuncName) + "upX_"); // a new temp Func at every level

	// 3x3 based Gaussian up
	upX(x, y) = img(x/2, y) + img((x+1)/ 2, y); // 10bit-> 11 bit
	gu(x, y)  = (upX(x, y/2) + upX(x, (y+1) / 2)) >> 2;  // 11bit-> 12bit -> 10bit

    return gu;
}
```

#### Func Definition inside a called function

```c++
Buffer<Y10Value_T> bufY(width, height);
// load bufY from file
Func aPyrLevels[] = { Func("GP_0"), Func("GP_1")};

BuildGaussianPyramid( bufY, width, height, aPyrLevels, NUM_PYR_LEVELS,"GPyr_");
```

called function defined here: note ==tentative local Func variables==.

```c++
void BuildGaussianPyramid(Buffer<Y10Value_T> img, Func * pPyrLevels, int numLevels, const char * namePrefix)
{
	Var x("x"), y("y");

	pPyrLevels[0] = BoundaryConditions::mirror_interior(img, 0, width, 0, height);

	for (int lvl = 1; lvl < numLevels; lvl++) {
        // OK for tentative local Func variable
		Func prev = pPyrLevels[lvl - 1];  // no need to clamp function
		Func downX(string(namePrefix) + "downX_" + to_string(lvl)); // a new temp Func at every level

		// 3x3 based Gaussian down
		downX(x, y) = u16((prev(2 * x - 1, y) + (prev(2 * x, y) << 1) + prev(2 * x + 1, y)));
		pPyrLevels[lvl](x, y) = ((downX(x, 2 * y - 1) + (downX(x, 2 * y) << 1) + downX(x, 2 * y + 1)) >> 4);
	}
}
```



## Expr

A fragment of [Halide](http://halide-lang.org/docs/namespace_halide.html) syntax. It's implemented as reference-counted ==handle== to a concrete expression node, but it's immutable, so you can treat it as a value type.

## Buffer

A [Halide::Buffer](http://halide-lang.org/docs/class_halide_1_1_buffer.html) is a *named shared* ==reference== to a [Halide::Runtime::Buffer](http://halide-lang.org/docs/class_halide_1_1_runtime_1_1_buffer.html).


## Boundary Handling

Only the input and out buffer need to handle boundary condition. `Func` doesn't need to and its arguments can take on any range that deems necessary. The following example from [Stack Overflow](HTTPS://STACKOVERFLOW.COM/QUESTIONS/43168885/IS-THERE-ANY-WAY-TO-COMBINE-FUNCS-INTO-A-FUNC-HAS-ONE-MORE-DIMENSION) illustrated this well

```c++
Var x, y;
Func f, g;
RDom range(0, 3, 0, 3); // Form is min/extent, not start/end

f(x, y) = 0; // Initial condition
// NO out-of-bound for Func 
f(range.x, range.y) = f(range.x - 1, range.y - 1) + 1;

// Wrapper necessary as the final stage is used to output of certain size
g(x, y) = f(x, y);
Buffer<int32t> result = g.realize(3, 3);
```

## AOT

To use AOT generated codes, 

- Include `HalideBuffer.h` to use `Halide::Runtime::Buffer`
- Include `HalideRuntime.h` (NOT `Halide.h`)

## Select

According to [Halide creator](see https://stackoverflow.com/questions/37733563/halideexpr-is-not-contextually-convertible-to-bool-storing-values-of-fun)

>  `select` is to Halide (an Expr which conditionally evaluates to one of two values) as if/else is to C (a statement which conditionally executes one of two sub-statements).

```c++
// select doc:
//   Returns an expression similar to the ternary operator in C, 
//   except that it always evaluates *all* arguments.
//  select is to Halide (an Expr which conditionally evaluates to one of two values) 
//  as if/else is to C (a statement which conditionally executes one of two sub-statements).
```

# Halide Types

All Halide **scalar types** in Halide:

```c++
Halide::Type valid_halide_types[] = {
     UInt(8), UInt(16), UInt(32), UInt(64),
     Int(8), Int(16), Int(32), Int(64),
     Float(32), Float(64), Handle()
};
```

- All **Exprs** have a scalar type

- all **Funcs** evaluate to one or *more* scalar types:

  - Multi-valued func: `f2(x) = {x, sin(x)};`

An Expr representing a Var has type Int(32): ==type()==
```c++
assert(Expr(x).type() == Int(32));
```

**Construct Halide type from C++ type**: ==type_of<>()==

```c++
assert(type_of<float>() == Float(32));
```

## Type casting

### Expr casting

You can cast an Expr from one Type to another using the cast operator:

```c++
assert(Halide::cast(UInt(8), x).type() == UInt(8));
```

This also comes in a template form that takes a ==C++== type.
```c++
assert(Halide::cast<uint8_t>(x).type() == UInt(8));
```

**`Halide::ConciseCasts`**:

```c++
 inline Expr f32(Expr e) {
     return cast(Float(32, e.type().lanes()), e);
 }
```

### Buffer casting

You can always cast anything to an Expr using: `Expr(x)`

```c++
Halide::Buffer<uint8_t> input = load_image("images/rgb.png");

Halide::Func brighter;
brighter(x, y, c) = Halide::cast<uint8_t>( min( input(x, y, c) * 1.5f, 255) );
```

