qcheckers Installation Procedures
=================================

Run the following commands:

```
qmake
make
make install
```

Shared files are installed in `/usr/local/share/qcheckers` by default. You can
change this by providing `PREFIX` variable to qmake:

```
qmake PREFIX=/usr
```

Technically, `PREFIX` is where qcheckers will look up for it's shared files;
and `INSTALL_ROOT` is where all files will be installed when you run `make
install`. So if you want `make install` to install to some temporary directory,
to actually install files later (for example, during building a package for one
of linux distros), you may say

```
qmake PREFIX=/usr INSTALL_ROOT=/tmp
```

