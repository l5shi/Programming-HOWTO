# Create Python Extensions

## Background

https://docs.python.org/2/distutils/apiref.html#distutils.core.Extension

## Set-up on Linux
```python
# Check for the default compiler
sysconfig.get_config_var('CC')
```

### Issues with `ld` Provided Anaconda
Anaconda provides an `ld` in `compiler_compat/` folder to work around some backward compatibilty issue ([details](https://groups.google.com/a/continuum.io/forum/#!topic/anaconda/1810eV8l7FU)). However, this `ld` can not find the standard lib's in the standard lib folder, e.g., `/usr/lib/x86_64-linux-gnu/`. To fix that, add the lib path to the **Extension** definition in `setup.py`. For example (pycocotool),
```python
ext_modules = [
    Extension(
        'pycocotools._mask',
        sources=['../common/maskApi.c', 'pycocotools/_mask.pyx'],
        include_dirs = [np.get_include(), '../common'],
        extra_compile_args=['-Wno-cpp', '-Wno-unused-function', '-std=c99'],
        extra_link_args=['-L/usr/lib/x86_64-linux-gnu/'],
    )
]
```

## Set-up on Windows

### Install Compiler

The compiler used to build extensions MUST be the same as that used to build the Python distribution. **not a problem for Linux??** 

[Info on VS versions for different Python versions](https://blogs.msdn.microsoft.com/pythonengineering/2016/04/11/unable-to-find-vcvarsall-bat/): see the table
 - [MS VC++ for Python 2.7](https://www.microsoft.com/en-us/download/details.aspx?id=44266): VS 2008 (VS9) installed at `C:\Users\<user_id>\AppData\Local\Programs\Common\Microsoft\Visual C++ for Python\9.0\`

### HACK's to make it work for Python 2.7

* Set environment variable `VS90COMNTOOLS` to `C:\Users\<user_id>\AppData\Local\Programs\Common\Microsoft\Visual C++ for Python\9.0\VC\bin`

* Hack `find_vcvarsall` in `distutils\msvc9compiler.py`: the following code snipet shows the HACK needed for it to find `vcvarsall.bat`

```python
    # productdir = os.path.join(toolsdir, os.pardir, os.pardir, "VC")
    # -----------------------------------------
    # Hack to make it work for "Visual C++ for Python 2.7"
    productdir = os.path.join(toolsdir, os.pardir, os.pardir)
```

* Hack to remove **duplicate** objects provided to linker: needed for some extensions, e.g., https://github.com/pathak22/pyflow/.


```python
    def link(self,...):
        if not self.initialized: self.initialize()
        
        # HACK to remove the last object in the list that is DUPLICATE (on Windows)
        #objects = objects[:-1]
        (objects, output_dir) = self._fix_object_args(objects, output_dir)
```