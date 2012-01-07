#----------------user/install options----------------
PREFIX = /usr/local/
BINDIR = $(PREFIX)bin/

#----------------compile options---------------------
CC = gcc
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

#----------------developer options-------------------
DFLAGS += $(FLAGS)
DFLAGS += -DDEBUG
DFLAGS += -ggdb
DFLAGS += -g

#----------------end of options----------------------
REAL_NAME = nyancat-console

SRCDIR = src

OBJ = $(patsubst %.c, %.o, $(wildcard *.c))
DOBJ = $(patsubst %.c, %.do, $(wildcard *.c))
HEADERS = $(wildcard *.h)
SOURCE = $(wildcard $(SRCDIR)/*.c)

TARGET = $(REAL_NAME)
DTARGET=$(TARGET)_debug

MAKE=make --no-print-directory
