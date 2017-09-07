# Create Python Extensions

## Set-up on Windows

### Install Compiler

The compiler used to build extensions MUST be the same as that used to build the Python distribution. 

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