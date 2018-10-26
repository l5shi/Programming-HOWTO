#include <iostream>
#include <fstream>
#include "Halide.h"

using namespace Halide;

bool load_raw_image(std::string &img_name, Buffer<uint16_t> &imgs, int width, int height)
{
    int img_buffer_size = width * height * sizeof(int16_t);
    
    imgs = Buffer<uint16_t>(width, height);
    char *data = (char *)imgs.data();
    
    std::ifstream is(img_name, std::ifstream::binary);
    if (is)
    {
        is.seekg(0, is.end);
        if(img_buffer_size != is.tellg())
        {
            std::cerr << " The raw file" << img_name << "size is not as specified" << std::endl;
            return false;
        }
        is.seekg (0, is.beg);
        is.read(data, img_buffer_size);
    }else
    {
        std::cerr << " cannot open the raw file " << img_name << std::endl;
        return false;
    }
    
    is.close();
    return true;
}

int main(int argc, char **argv)
{
    std::string file_name("path to the image/payload_N000.raw");
    Buffer<uint16_t> input_data;

    const int T_WIDTH = 16, T_HEIGHT = 16, C = 4, WIDTH = 4208, HEIGHT = 3120; // specify the tile dimession T_WIDTH, T_HEIGHT; channel number C; image WIDTH and HEIGHT.
    
    if (!load_raw_image(file_name, input_data, WIDTH, HEIGHT)) // load raw image
        return -1;
    std::cout << "load success!" << std::endl;
    
    Var tx("tx"), ty("ty"), c("c"), ix("ix"), iy("iy"); // tx and ty are tile index; c is channel index, ix and iy are a pixels relative index within a tile in x and y direction respecitively.
    Func tiled_img("tiled_img");
    tiled_img(ix, iy, c, tx, ty) = input_data(2*ix + T_WIDTH * 2 * tx + c % 2, 2 * iy + T_HEIGHT * 2 * ty + c / 2);
    Func tiled_img_float("tiled_img_float");
    tiled_img_float(ix, iy, c, tx, ty) = Halide::cast<float>(tiled_img(ix, iy, c, tx, ty));
    
    // call fft here fft_tiled_img = fft(tiled_img(ix, iy, c, tx, ty), T_WIDTH, T_WIDTH);
    Target target = get_jit_target_from_environment();
    Fft2dDesc fwd_desc;
    Fft2dDesc inv_desc;
    ComplexFunc dft_data = fft2d_r2c(tiled_img_float, T_WIDTH, T_HEIGHT, target, fwd_desc);
    dft_data.compute_root();
    dft_data.realize(std::vector<int>{T_WIDTH, T_HEIGHT/2 + 1, C, WIDTH/T_WIDTH/2, HEIGHT/T_HEIGHT/2});
    
    return 0;
}
