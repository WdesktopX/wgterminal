# LXTerminal fork

Starting point:
commit 0febe16c2f97d75df04936df4ef34667655f084a
Date:   Fri Dec 3 16:49:59 2021 -0500
    Allow tab width to be set via "Edit -> Preferences -> Advanced".

LXTerminal is a VTE-based terminal emulator with support for multiple tabs.  It
is completely desktop-independent and does not have any unnecessary
dependencies. In order to reduce memory usage and increase the performance all
instances of the terminal are sharing a single process.

## Building and installation

### Dependencies

This dependency is listed as the package name used in Debian.  If your
distribution is neither Debian nor its derivatives, you could find equivalents
for your target distribution.

Basic requirements for building:
* libglib2.0-dev
* libgtk2.0-dev
* libvte-dev

### Compile and install

You may try:

```
./configure --prefix=/usr
make
sudo make install
```
