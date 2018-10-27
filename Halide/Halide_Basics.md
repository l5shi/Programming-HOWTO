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

### Buffer to Func

```c++
Halide::Func::Func(Buffer< T > & im)
```

Construct a new [Func](http://halide-lang.org/docs/class_halide_1_1_func.html) to wrap a [Buffer](http://halide-lang.org/docs/class_halide_1_1_buffer.html). (from Halide doc)

## Expr

A fragment of [Halide](http://halide-lang.org/docs/namespace_halide.html) syntax. It's implemented as reference-counted ==handle== to a concrete expression node, but it's immutable, so you can treat it as a value type.

## Buffer

A [Halide::Buffer](http://halide-lang.org/docs/class_halide_1_1_buffer.html) is a *named shared* ==reference== to a [Halide::Runtime::Buffer](http://halide-lang.org/docs/class_halide_1_1_runtime_1_1_buffer.html).

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



