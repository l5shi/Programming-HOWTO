
# Jupyter Notebook

## Remote Access via SSH Tunneling

See [Remote Access to IPython Notebooks via SSH](https://coderwall.com/p/ohk6cg/remote-access-to-ipython-notebooks-via-ssh)

On **remote** machine:
```shell
jupyter notebook --no-browser --port=8889

jupyter notebook list  # list currently running server
```


On **local** machine:
```sh
# -N: no remote command to be executed
# -f: tell SSH to go to background
# -L: list the port forwarding config: remote port 8889 to local port 8888
ssh -N -f -L localhost:8888:localhost:8889 remote_user@remote_host
```

In browser: `localhost:8888`