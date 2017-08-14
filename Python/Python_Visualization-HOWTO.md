<!-- TOC -->

- [Bokeh](#bokeh)
- [Vispy](#vispy)
- [VPython](#vpython)
    - [Examples](#examples)
    - [Glowscript: VPython's younger sister](#glowscript-vpythons-younger-sister)
- [PyGeo: Python Geometry](#pygeo-python-geometry)

<!-- /TOC -->

[The future of visualization in Python - are we going where we want to be?](http://www.almarklein.org/future_vis.html)

Feature | Bokeh | VisPy | Matplotlib
---------|----------|---------|---------
 **Rendeing** | Javascript | OpenGL | various
 **Strength** | 2D, Browser support | 3D | publication quality plots
 **Weakness** | 3D | Browser support| slow(?)


# Bokeh

[Key Concepts](http://bokeh.pydata.org/en/latest/docs/user_guide/concepts.html)

Python script using Bokeh to generate a HTML file that will be rendered by a browser

```python
from bokeh.plotting import figure, show, output_file
from bokeh.sampledata.iris import flowers
colormap = {'setosa': 'red', 'versicolor': 'green', 'virginica': 'blue'}
colors = [colormap[x] for x in flowers['species']]
p = figure(title = "Iris Morphology")
p.xaxis.axis_label = 'Petal Length'
p.yaxis.axis_label = 'Petal Width'
p.circle(flowers["petal_length"], flowers["petal_width"],
         color=colors, fill_alpha=0.2, size=10)
output_file("iris.html", title="iris.py example")
show(p) # open "iris.html" in browser
```

# Vispy

# VPython
http://vpython.org/
Generate navigable real-time 3D annimation.

 Starting with VPython 6, it is based on the cross-platform library **wxPython**, hence the project name VPython-wx. See [announcement](http://vpython.org/contents/new_features.html):
 >The major changes from VPython 5 made possible by wxPython are 
 - the elimination of nearly all platform--dependent C++ code
 - the elimination of complex multithreading in the C++ code, 
 - and the ability for Macintosh versions to use 64-bit Python based on the Cocoa framework (VPython 5 on the Mac was based on the Carbon framework, which is no longer supported by Apple).

 [**Future of VPython**](https://github.com/BruceSherwood/vpython-wx)
 >This incarnation of VPython will not be developed further; see this announcement for an explanation and this history of the development of VPython. Development efforts are now focused on a Jupyter notebook-based version **vpython-jupyter** and an in-browser version that requires no local installation at all and runs on mobile devices, at **glowscript.org**.

## Examples

Lot of YouTube video showing VPython examples, e.g., Earth orbiting around Sun.

## Glowscript: VPython's younger sister
display 3D animations on a Web page

A related project is GlowScript (glowscript.org ), which provides similar ease of use for JavaScript or CoffeeScript programs, which run in a browser.

# PyGeo: Python Geometry

http://pygeo.sourceforge.net/
Use VPython and numpy