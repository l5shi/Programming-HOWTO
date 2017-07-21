matplotlib HOWTO

# Animation

## Update `imshow` contents

Use `set_data` method
```python
myobj = imshow(first_image) # return matplotlib.image.AxesImage
for pixel in pixels:
    addpixel(pixel)
    myobj.set_data(segmentedimg)
    draw() # make sure that the backend updates the image.
```

## `matplotlib.animation`
https://stackoverflow.com/questions/17835302/how-to-update-matplotlibs-imshow-window-interactively
```python
import numpy as np
from matplotlib import pyplot as plt
from matplotlib import animation

nx = 150
ny = 50

fig = plt.figure()
data = np.zeros((nx, ny))
im = plt.imshow(data, cmap='gist_gray_r', vmin=0, vmax=1)

def init():
    im.set_data(np.zeros((nx, ny)))

def animate(i):
    xi = i // ny
    yi = i % ny
    data[xi, yi] = 1
    im.set_data(data)
    return im

anim = animation.FuncAnimation(fig, animate, init_func=init, frames=nx * ny,
                               interval=50)
plt.show()
```

# matplotlib Widgets

```python
import numpy as np
from matplotlib import pyplot as plt
from matplotlib.widgets import Slider

# generate a five layer data
data = np.random.randint(10, size=(5, 5, 5))
# current layer index start with the first layer
idx = 0

# figure axis setup
fig, ax = plt.subplots()
fig.subplots_adjust(bottom=0.15)

# display initial image
im_h = ax.imshow(data[:, :, idx], cmap='hot', interpolation='nearest')

# setup a slider axis and the Slider
ax_depth = plt.axes([0.23, 0.02, 0.56, 0.04])
slider_depth = Slider(ax_depth, 'depth', 0, data.shape[2]-1, valinit=idx)

# update the figure with a change on the slider
def update_depth(val):
    idx = int(round(slider_depth.val))
    im_h.set_data(data[:, :, idx])

slider_depth.on_changed(update_depth)

plt.show()
```
