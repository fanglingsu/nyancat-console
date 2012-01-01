REAL_NAME = nyancat-console
CC = gcc

SRCDIR = src

FLAGS += -pedantic
FLAGS += -Wall
FLAGS += -Wmissing-declarations
FLAGS += -Wmissing-parameter-type
FLAGS += -Wstrict-prototypes
FLAGS += -pipe
FLAGS += -O3
FLAGS += -std=c99
FLAGS += -D_GNU_SOURCE
LIBS = -lncurses

DFLAGS += $(FLAGS)
DFLAGS += -DDEBUG
DFLAGS += -ggdb
DFLAGS += -g


OBJ = $(patsubst %.c, %.o, $(wildcard *.c))
DOBJ = $(patsubst %.c, %.do, $(wildcard *.c))
HEADERS = $(wildcard *.h)
SOURCE = $(wildcard $(SRCDIR)/*.c)

TARGET = $(REAL_NAME)
DTARGET=$(TARGET)_debug

MAKE=make --no-print-directory
