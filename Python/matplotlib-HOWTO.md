matplotlib HOWTO
<!-- TOC -->

- [Concepts](#concepts)
- [Animation](#animation)
    - [Update `imshow` contents](#update-imshow-contents)
    - [`matplotlib.animation`](#matplotlibanimation)
- [matplotlib Widgets](#matplotlib-widgets)

<!-- /TOC -->

# Concepts
http://matplotlib.org/2.0.0/glossary/index.html
Great article on speed-up animation explains how matplotlib works: https://stackoverflow.com/questions/8955869/why-is-plotting-with-matplotlib-so-slow
http://bastibe.de/2013-05-30-speeding-up-matplotlib.html

**Steps**:
* **Figure**: data structure holds plotting objects such as ax, lines and etc. Plotting objects can be modified by `setp`, `set_data`, `set_ydata` and etc.
  * **ax** contains **artists** accessed using `ax.get_children()`. Example artists:
    * background `ax.patch`: `ax.draw_artist(ax.patch)`
    * `ax.title`
    * line returned by `plot()` function
    * the spines `ax.spines`: Spines are the lines connecting the axis tick marks and noting the boundaries of the data area. See [example](https://matplotlib.org/examples/ticks_and_spines/spines_demo.html)
    * the axes `ax.xaxis` and `ax.yaxis`: tick marks/labels and etc.

```python
fig, ax = plt.subplots()
```

* **Canvas**: *contains* figure (`fig.canvas`)
  * **Draw/Render** _existing_ plotting objects on Canvas: `ax.draw_artist(line)`
  * Copy rendered lines to the drawing backend: `fig.canvas.update()`
    * it is as fast as `fig.canvas.blit(ax.bbox)` but the latter leaks memory like crazy. 
  * `fig.canvas.flush_events()` : w/o calling this, the plot is blank  

```python
# In wxPython, Canvas is derived from wx.Panel: https://matplotlib.org/api/backend_wxagg_api.html
FigureCanvasWxAgg(parent, id, figure)
```
* **Show** plot window: `plt.show()`

An example of fast re-drawing in annimation
```python
    numPts = 4000
    line, = ax.plot(np.random.randn(numPts))  # create and draw plotting objects
    fig.canvas.draw()  # draw_artist below can only be used after an initial 
                       # draw which caches the render 
    # save what has been drawn: MUST after draw()
    staticPart = fig.canvas.copy_from_bbox(ax.bbox) 

    vl, = ax.plot((0,0), (-4, 4), 'b')
    plt.show(block=False)  # show the plot window

    tstart = time.time()
    for t in xrange(numPts):
        fig.canvas.restore_region( staticPart ) # restore static partE

        vl.set_xdata((t,t))
        ax.draw_artist(vl)
        fig.canvas.update() # copy the newly rendered lines to the drawing backend.
        # MUST use canvas.blit( ) with wx: transfer the region of the agg
        # buffer defined by bbox to the display
        # see https://matplotlib.org/api/backend_wxagg_api.html

        fig.canvas.flush_events() # needed: otherwise the plot is blank
```


# Animation

real-time plotting in while loop with matplotlib
https://stackoverflow.com/questions/11874767/real-time-plotting-in-while-loop-with-matplotlib
```python
plt.axis([0, 10, 0, 1])
plt.ion() # enable interactive

for i in range(10):
    y = np.random.random()
    plt.scatter(i, y) # better to use setp?
    plt.pause(0.05) #draw the new data and run GUI event loop => like cv2.waitKey()?

while True:
    plt.pause(0.05)
```

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
