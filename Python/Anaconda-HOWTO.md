# Conda

## Environments

A conda environment is a directory that contains a specific collection of conda packages that you have installed. For example, you may have one environment with NumPy 1.7 and its dependencies, and another environment with NumPy 1.6 for legacy testing.

https://conda.io/docs/user-guide/concepts.html#conda-environments

https://conda.io/docs/user-guide/tasks/manage-environments.html

By default, environments are installed into the `envs` directory in your conda directory, e.g., `C:\apps\Anaconda2\envs\py36`

**TIPS**
- In Windows, it is good practice to deactivate one environment before activating another.

**Create Environments**

```shell
conda create -n py36 python=3.6 anaconda
```

* name `py36`
* `anaconda` is the metapackage that includes all of the Python packages comprising the Anaconda distribution. 
* packages and versions to install in this new environment: `python=3.6`, `numpy=1.7`, or multiple packages.

**List Environments**
```shell
conda info --envs
```
In the environments list that displays, your _current_ environment is highlighted with an asterisk (*).

### Activate/Deactiavte

`activate` and `deactivate` are separate commands. NOT `conda` commands.

It basically prepends the environment dir to the `PATH` environment variable, which ONLY works for `cmd.exe` on Windows (not ConsoleZ.exe, PowerShell and etc. See https://github.com/conda/conda/issues/1519 for possible fix)

## Install Different Python Versions
[Use Conda to manage multiple python versions](https://conda.io/docs/user-guide/tasks/manage-python.html#installing-a-different-version-of-python)