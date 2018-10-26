# Working with C/C++

|         | Halide                                                       | C/C++ |
| ------- | ------------------------------------------------------------ | ----- |
| **AOT** | AOT routine's arguments are usually<br> - `Params`<br> - `ImageParams` |       |
| **`define_extern`** | `ExternFuncArgument` may be<br>- `Func`<br>- `Expr`<br>- `Buffer` or `ImageParams` (AOT) | |




## Call AOT from C/C++

For AOT-compiled code, we need to explicitly declare the arguments to the routine. Arguments are usually:

- `Params`
- `ImageParams`

```c++
// see below for detailed definitions
ImageParam input(type_of<uint8_t>(), 2);
Param<uint8_t> offset;
brighter(x, y) = input(x, y) + offset;
brighter.compile_to_static_library()"lesson_10_halide", {input, offset}, "brighter");

// prototype of AOT'ed function
int brighter(halide_buffer_t *_input_buffer, uint8_t _offset, halide_buffer_t *_brighter_buffer);
```

| JIT  | AOT  |
| ---- | ---- |
| Buffer     |  ImageParam    |
| Expr    |  Param    |

Tutorial lesson 10


```c++
    // The pipeline will depend on one scalar parameter.
    Param<uint8_t> offset;
    // 1st constructor argument gives the type of a pixel, and the 2nd
    // specifies the number of dimensions.
	// Currently, four dimensions is the maximum for inputs and outputs.
    ImageParam input(type_of<uint8_t>(), 2);
    // If we were jit-compiling, these would just be an int and a
    // Buffer, but because we want to compile the pipeline once and
    // have it work for any value of the parameter, we need to make a
    // Param object, which can be used like an Expr, and an ImageParam
    // object, which can be used like a Buffer.
    // Define the Func.
    brighter(x, y) = input(x, y) + offset;
```

## Call C functions from Halide

`ExternFuncArgument` may be

- `Func`
- `Buffer` or `ImageParam` (for AOT)
- `Expr`

int argument type: 

- `test/correctness/skip_stages_external_array_functions.cpp`
- `test/auto_schedule/extern.cpp`

```c++
f2.define_extern("call_counter", {f1, 1, 0}, UInt(8), 1);

std::vector<ExternFuncArgument> args(3);
    args[0] = f0;
    args[1] = Expr(3);
    args[2] = Expr(7);

```





# Halide Apps

## Gaussian Pyramid Image Interpretation

See [Gaussian Pyramid Image Interpretation](http://sepwww.stanford.edu/data/media/public/docs/sep124/ssen1/paper_html/node8.html#SECTION00040000000000000000) to recover missing part.

Halide example uses 3x3 Gaussian filter.

## FFT

### Halide FFT test code



```c++
void test14(){
    const int W = 4032, H = 3024, N1 = 5;
    const int tile_size = 16;
    const int half_tile_size = tile_size / 2;
    const int TX = W / half_tile_size - 1;
    const int TY = H / half_tile_size - 1;
    Var x("x"), y("y"), xi("xi"), yi("yi"), tx("tx"), ty("ty"), n("n");
    Buffer<float> in(W, H, N1);
    for(int n1 = 0; n1 < N1; n1++){
        for(int h = 0; h < H; h++){
            for(int w = 0; w < W; w++){
                in(w, h, n1) = (float)rand()/(float)RAND_MAX;
            }
        }
    }
    
    Func r2c_in;
    r2c_in(xi, yi, tx, ty, n) = in(xi + tx * half_tile_size, yi + ty * half_tile_size, n);
    
    Target target = get_jit_target_from_environment();
    Fft2dDesc fwd_desc;
    Fft2dDesc inv_desc;
    inv_desc.gain = 1.0f/(tile_size * tile_size);
    ComplexFunc dft_data = fft2d_r2c(r2c_in, tile_size, tile_size, target, fwd_desc);
    dft_data.compute_root();
    Func idft_data;
    idft_data = fft2d_c2r(dft_data, tile_size, tile_size, target, inv_desc);
    std::vector<int> dim_vec;
    dim_vec.push_back(tile_size);
    dim_vec.push_back(tile_size);
    dim_vec.push_back(TX);
    dim_vec.push_back(TY);
    dim_vec.push_back(N1);
    
    
    idft_data.realize(dim_vec, target);
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
//    Buffer<float> result_r2c = filtered_r2c.realize(W, H, target);
    Buffer<float> result_r2c = idft_data.realize(dim_vec, target);
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
    std::cout << "It took " << time_span.count() << " seconds." << std::endl;
    
    std::cout << "dim 0 = "<<result_r2c.extent(0) << std::endl;
    std::cout << "dim 1 = "<<result_r2c.extent(1) << std::endl;
    std::cout << "dim 2 = "<<result_r2c.extent(2) << std::endl;
    std::cout << "dim 3 = "<<result_r2c.extent(3) << std::endl;
    std::cout << "dim 4 = "<<result_r2c.extent(4) << std::endl;
    
    for(int n2 = 0; n2 < N1; n2++)
    {
        for(int ity = 0; ity < TY; ity++){
            for(int itx = 0; itx < TX; itx++){
                for(int iyi = 0; iyi < tile_size; iyi++){
                    for(int ixi = 0; ixi < tile_size; ixi++){
                        if(fabs(in(ixi + itx * half_tile_size, iyi + ity * half_tile_size, n2) - result_r2c(ixi, iyi, itx, ity, n2)) > 1e-6){
                            std::cout << "(" << in(ixi + itx * half_tile_size, iyi + ity * half_tile_size,n2) << ", " << result_r2c(ixi, iyi, itx, ity, n2) << ")" << std::endl;
                        }
                    }
                }
            }
        }
    }
    
//    std::ofstream outbin_input(input_data_file, std::ios::out | std::ios::binary);
//    const char *temp0 = (char *)in.data();
//    outbin_input.write(temp0, W * H * sizeof(float));
//    outbin_input.close();
//    std::ofstream outbin_output(fft_out_file, std::ios::out | std::ios::binary);
//    const char *temp1 = (char *)result_r2c.data();
//    outbin_input.write(temp1, tile_size * tile_size * TX * TY * sizeof(float) * 2);
//    outbin_input.close();
    
    std::cout << "Success!" << std::endl;
}
```