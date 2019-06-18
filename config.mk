# see LICENSE file for copyright and license details.

# version
VERSION = 0.1

# paths
PREFIX = /usr/local
MANPREFIX = ${PREFIX}/share/man

# flags
CPPFLAGS = -DVERSION=\"${VERSION}\"
CFLAGS = -std=c99 -pedantic -Wall -Wextra -Werror -Os -I. -I/usr/X11R6/include
LDFLAGS = -s -L/usr/X11R6/lib -lX11

# compiler and linker
CC = cc

