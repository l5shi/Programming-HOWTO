
# Concepts

## Mat

[OpenCV 2.4 doc on Mat](https://docs.opencv.org/2.4/doc/tutorials/core/mat_the_basic_image_container/mat_the_basic_image_container.html?highlight=mat#mat
)
> It has two data parts:
>  - header: fixed size
>  - matirx

[Access indivisual pixel value](https://stackoverflow.com/questions/7899108/opencv-get-pixel-channel-value-from-mat-image)

```c++
Mat foo = imread("bar.png");

//Assuming the type is CV_8UC3 you would do this:
for(int i = 0; i < foo.rows; i++)
    for(int j = 0; j < foo.cols; j++) {
        Vec3b bgrPixel = foo.at<Vec3b>(i, j);
        // do something with BGR values...
    }

// Use direct access to speed it up
uint8_t* pixelPtr = (uint8_t*)foo.data;
int cn = foo.channels();
Scalar_<uint8_t> bgrPixel;

for(int i = 0; i < foo.rows; i++) // each row is stored continuously
    for(int j = 0; j < foo.cols; j++)
    {
        bgrPixel.val[0] = pixelPtr[i*foo.cols*cn + j*cn + 0]; // B
        bgrPixel.val[1] = pixelPtr[i*foo.cols*cn + j*cn + 1]; // G
        bgrPixel.val[2] = pixelPtr[i*foo.cols*cn + j*cn + 2]; // R

        // do something with BGR values...
    }
```

`modules/core/include/opencv2/core/core.hpp`
```c++
// line
class CV_EXPORTS Mat {  // line 1708: definition

}

#include "opencv2/core/mat.hpp"  // line 4922: Mat's implementation
```