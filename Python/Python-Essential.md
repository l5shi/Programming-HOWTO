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