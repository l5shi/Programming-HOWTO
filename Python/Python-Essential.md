<!-- TOC -->

- [File I/O](#file-io)
- [Functions are Objects](#functions-are-objects)
    - [Define your own function object class](#define-your-own-function-object-class)
- [Extend Basic Class](#extend-basic-class)
    - [Extend `dict`](#extend-dict)

<!-- /TOC -->

# File I/O

Read from stdin: 
```python
for line in sys.stdin:
```

Perl's `while(<>)` like interface:
```python
import fileinput
for line in fileinput.input():
```
`fileinput` will loop through all the lines in the input specified as file names given in command-line arguments, or the standard input if no arguments are provided

# Functions are Objects

In Python,  functions are objects.
`foo(x)` is equivalent to `foo.__call__(x)`

## Define your own function object class

```python
class TimedFunction:
    def __init__(self, func, funcName, numRunsInReportBatch= 30):
        self.func, self.funcName, self.numRunsInReportBatch = func, funcName, numRunsInReportBatch
        self.numRuns, self.sumRunTime = 0, 0.0

    def __call__(self, *args, **kwargs):
        startTime = time.time()

        self.func( *args, **kwargs)

        self.sumRunTime += (time.time() - startTime)
        self.numRuns += 1
        if self.numRuns >= self.numRunsInReportBatch:
            print('{}\'s avg run time: {} ms'.format( self.sumRunTime / self.numRuns * 1000.0))
            self.numRuns, self.sumRunTime = 0, 0.0
```

# Extend Basic Class

## Extend `dict`

```python
class KeyCreatitonOrderDict (dict) :
    """Class doc """
    def __init__(self, *args, **kwArgs):
        self.keyList = []
        return super(KeyCreatitonOrderDict, self).__init__(*args, **kwArgs)

    #override [] operator
    def __getitem__(self, key):
        if key in self:
            return super(KeyCreatitonOrderDict, self).__getitem__(key)
        else:
            raise KeyError()

    def __setitem__(self, key, value):
        if key not in self:
            self.keyList.append(key)
        super(KeyCreatitonOrderDict, self).__setitem__(key, value)

    def keys(self):
        return self.keyList
```        