<!-- TOC -->

- [Build OpenCV](#build-opencv)
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

To use opencv, specify `OpenCV_DIR` as `<OpenCV_Root>/build`

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
