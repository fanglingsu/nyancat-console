REAL_NAME = nyancat

SRCDIR = src

FLAGS += -pedantic
FLAGS += -Wall
FLAGS += -Wno-long-long
FLAGS += -Wmissing-declarations
FLAGS += -Wstrict-prototypes
FLAGS += -ansi
FLAGS += -pipe
FLAGS += -O3
FLAGS += -std=c99
FLAGS += -D_POSIX_SOURCE
FLAGS += -D_BSD_SOURCE
FLAGS += -D_XOPEN_SOURCE_EXTENDED
LIBS = -lncurses

DFLAGS += $(FLAGS)
DFLAGS += -ggdb
DFLAGS += -g


OBJ = $(patsubst %.c, %.o, $(wildcard *.c))
DOBJ = $(patsubst %.c, %.do, $(wildcard *.c))
SOURCE = $(wildcard $(SRCDIR)/*.c)

TARGET = $(REAL_NAME)
DTARGET=$(TARGET)_debug

MAKE=make --no-print-directory
