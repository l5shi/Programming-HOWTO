<!-- TOC -->

- [Build OpenCV](#build-opencv)
    - [Example: build opencv 3.3 on Ubuntu 16.04](#example-build-opencv-33-on-ubuntu-1604)
- [GUI Working with OpenCV](#gui-working-with-opencv)
- [Video Processing](#video-processing)
- [Homography with OpenCV](#homography-with-opencv)
- [Animation](#animation)

<!-- /TOC -->

From OpenCV 2.X OpenCV 3.0 a few things changed. Specifically:
* `cv2.cv` doesn't exists in OpenCV 3.0. Use simply `cv2`
* `cv2.cv.CV_CAP_XXX` becomes `cv2.CAP_XXX`
* some defines changed, e.g. `CV_BGR2HSV` is now `COLOR_BGR2HSV`.

Find version info from source codes:
>In C++ several macros are defined to easily detect the version — `CV_VERSION`,...

>`grep -r --include="*.hpp" CV_VERSION *` 

# Build OpenCV

use cmake: straightforward.

To use opencv, specify `OpenCV_DIR` as `<OpenCV_Root>/build/x64/vc15/lib`  See the end of this [OpenCV Issue](https://github.com/opencv/opencv/issues/6132)



## Example: build opencv 3.3 on Ubuntu 16.04

```sh
1.	Download OpenCV3.3 source from (https://github.com/opencv/opencv/releases/tag/3.3.0)
2.	Cd <OpenCV directory>
3.	Mkdir build && cd build
4.	Mkdir linux && cd linux
cmake ../../ -DHAVE_LIBV4L=OFF -DCMAKE_BUILD_TYPE=RELEASE -DWITH_1394=OFF -DWITH_GTK=OFF -DWITH_JASPER=OFF -DBUILD_JPEG=ON -DBUILD_ZLIB=ON -DBUILD_SHARED_LIBS=OFF -DWITH_OPENEXR=OFF -DWITH_WEBP=OFF -DWITH_PNG=OFF -DWITH_TIFF=OFF -DHAVE_CAM4L2=OFF -DHAVE_VIDEOIO=OFF -DWITH_ITT=OFF -DBUILD_TESTS=OFF -DBUILD_PERF_TESTS=OFF -DBUILD_opencv_flann=OFF -DBUILD_opencv_shape=OFF -DBUILD_opencv_dnn=OFF -DBUILD_PACKAGE=OFF -DBUILD_PROTOBUF=OFF -DBUILD_WITH_DEBUG_INFO=OFF -DBUILD_opencv_apps=OFF -DWITH_GPHOTO=OFF -DWITH_GSTREAMER=OFF -DWITH_OPENCL=OFF -DWITH_MATLAB=OFF -DWITH_FFMPEG=OFF -DBUILD_opencv_videoio=OFF -DBUILD_opencv_videostab=OFF -DBUILD_opencv_video=OFF -DWITH_CUDA=OFF  -DCV_TRACE=OFF
```

# GUI Working with OpenCV

* Qt: use OpenGL
* ImGui: hosted on gitgub
* [cvui](http://www.learnopencv.com/cvui-gui-lib-built-on-top-of-opencv-drawing-primitives/): C++ header only cross-platform UI lib. [Github location](https://github.com/Dovyski/cvui)
* wxPython
  * https://stackoverflow.com/questions/14804741/opencv-integration-with-wxpython
  * https://stackoverflow.com/questions/35009984/get-stream-from-webcam-with-opencv-and-wxpython
  * https://stackoverflow.com/questions/37961949/record-webcam-video-with-wxpython-open-cv-using-control-buttons
  * https://stackoverflow.com/questions/24754265/displaying-videocapture-using-opencv-with-python-and-wxpython-in-a-wx-panel-sho

# Video Processing

cv2 based Video Player: https://github.com/maximus009/VideoPlayer/blob/master/new_test_3.py#L27
```python
# specify which video frame to read
cap.set(cv2.cv.CV_CAP_PROP_POS_FRAMES, i)
```
# Homography with OpenCV

http://www.learnopencv.com/homography-examples-using-opencv-python-c/


# Animation

```python
import cv2
import numpy as np
import time

Frame_out = np.zeros((500, 640, 4),np.uint8)

a = 1

while a<255:
    # OpenCV uses BGR
    cv2.rectangle(Frame_out,(a,a),(a*2,a*2),(0,0,255-a),0)
    time.sleep(0.05)
    cv2.imshow('Animation', Frame_out)
    cv2.rectangle(Frame_out,(a,a),(a*2,a*2),(0,0,0),0)
    a +=2
    if(a > 254):
        a = 1
    k = cv2.waitKey(10)
    if k == 27: break  # ESC to exit

cv2.destroyAllWindows()
```
