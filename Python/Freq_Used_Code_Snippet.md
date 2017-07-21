
# Python & OS Environments

## Python Environments

```python
IS_PY2 = sys.version_info < (3, 0)
```

## OS environment

```python
os.environ["PATH"]
```

# File Management

## Check if it is an executiable

```python
os.path.isfile(fpath) and os.access(fpath, os.X_OK)
```

```python
# splitext split the file name to two parts: the 2nd part is the file name extension
fnmwox = os.path.splitext(args.json_file)[0]
exe_file = os.path.join(path, program)

fpath, fname = os.path.split(program)
if fpath:
    if is_exe(program):
        return program
else:
    for path in os.environ["PATH"].split(os.pathsep):
        path = path.strip('"')
        exe_file = os.path.join(path, program)

```

# String Processing
Split Text
```python
#If sep is given, consecutive delimiters are not grouped
#together and are deemed to delimit empty strings (e.g.,
# '1,,2'.split(',') returns ['1', '', '2']).
#If sep is not specified or is None,  runs of consecutive
#whitespace are regarded as a single separator,
a = str.split(seq)

a = re.split(r'\s*:', line)
```

# Debug

## assert
assert

## Logging

```python
import logging

logging.basicConfig(level=logging.DEBUG,
                    format='(%(threadName)-10s) %(message)s',
                    )
logging.debug('Starting')
```
# Parallel Processing

`multiprocessing` is a package that supports spawning **processes** using an API similar to the `threading` module.

 `multiprocessing.dummy` replicates the API of multiprocessing but is no more than a wrapper around the `threading` module, e.g., `multiprocessing.dummy.ThreadPool`.

```python
# daemon does not exit
self.daemon = True
```

## `multiprocessing.dummy`

### `multiprocessing.dummy.ThreadPool`
[Simple example](https://www.codementor.io/lance/simple-parallelism-in-python-du107klle)

## `concurrent.futures` module in Python 3.x

http://masnun.com/2016/03/29/python-a-quick-introduction-to-the-concurrent-futures-module.html

## Queue: a multi-thread safe  data structure

Locking is handled **for** the caller.

```python
if IS_PY2:  from Queue import Queue
else:       from queue import Queue
```

[A Queue based keyboard interruptable thread pool ](https://www.metachris.com/2016/04/python-threadpool/)
