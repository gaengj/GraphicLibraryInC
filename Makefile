# Variable definitions.

PLATFORM	= $(shell uname)
CC		= gcc
OBJDIR		= ./objs
SRCDIR    = ./src
TESTS		= ./tests
INCLUDES	= ./include
INCFLAGS	:= -I${INCLUDES} -I${TESTS}
#OPTFLAGS	:= -Os -DNDEBUG
OPTFLAGS	:= -ggdb
CCFLAGS		:= -c ${OPTFLAGS} -Wall -std=c99

# The list of objects to include in the library

LIBEIOBJS	:=  ${OBJDIR}/ei_draw.o ${OBJDIR}/ei_button.o ${OBJDIR}/ei_application.o ${OBJDIR}/ei_widget.o ${OBJDIR}/ei_placer.o ${OBJDIR}/ei_widgetclass.o ${OBJDIR}/ei_toplevel.o




# Platform specific definitions (OS X, Linux)

ifeq (${PLATFORM},Darwin)

	# Building for Mac OS X

	PLATDIR		= _osx
	INCFLAGS	:= ${INCFLAGS} -I/opt/local/include/SDL
	LINK		= ${CC}
	LIBEI		= ${OBJDIR}/libei.a
	LIBEIBASE	= ${PLATDIR}/libeibase.a
	LIBS		= ${LIBEIBASE} -L/opt/local/lib -lSDL -lSDL_ttf -lSDL_image -framework AppKit
	CCFLAGS		:= ${CCFLAGS} -D__OSX__

else

	# Building for Linux

	PLATDIR		= _x11
	INCFLAGS	:= ${INCFLAGS} -I/usr/include/SDL
	LINK		= ${CC}
#	ARCH	        = 32
	ARCH	        = 64
	LIBEI		= ${OBJDIR}/libei.a
	LIBEIBASE	= ${PLATDIR}/libeibase${ARCH}.a
	LIBS		= ${LIBEIBASE} -lSDL -lSDL_ttf -lSDL_image -lm
	CCFLAGS		:= ${CCFLAGS} -D__LINUX__ -m${ARCH}
	LDFLAGS		= -m${ARCH} -g

endif



# Main target of the makefile. To build specific targets, call "make <target_name>"

TARGETS		=	${LIBEI} \
			minimal lines frame toplevel test_button button hello_world puzzle two048
all : ${TARGETS}


# ei_draw
${OBJDIR}/ei_draw.o : ${SRCDIR}/ei_draw.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${SRCDIR}/ei_draw.c -o ${OBJDIR}/ei_draw.o

# ei_button
${OBJDIR}/ei_button.o : ${SRCDIR}/ei_button.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${SRCDIR}/ei_button.c -o ${OBJDIR}/ei_button.o

# ei_application
${OBJDIR}/ei_application.o : ${SRCDIR}/ei_application.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${SRCDIR}/ei_application.c -o ${OBJDIR}/ei_application.o

# ei_widget
${OBJDIR}/ei_widget.o : ${SRCDIR}/ei_widget.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${SRCDIR}/ei_widget.c -o ${OBJDIR}/ei_widget.o

# ei_widgetclass
${OBJDIR}/ei_widgetclass.o : ${SRCDIR}/ei_widgetclass.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${SRCDIR}/ei_widgetclass.c -o ${OBJDIR}/ei_widgetclass.o

# ei_placer
${OBJDIR}/ei_placer.o : ${SRCDIR}/ei_placer.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${SRCDIR}/ei_placer.c -o ${OBJDIR}/ei_placer.o

# ei_toplevel
${OBJDIR}/ei_toplevel.o : ${SRCDIR}/ei_toplevel.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${SRCDIR}/ei_toplevel.c -o ${OBJDIR}/ei_toplevel.o


########## Test-programs



# minimal

minimal : ${OBJDIR}/minimal.o ${LIBEIBASE}
	${LINK} -o minimal ${LDFLAGS} ${OBJDIR}/minimal.o ${LIBS}

${OBJDIR}/minimal.o : ${TESTS}/minimal.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/minimal.c -o ${OBJDIR}/minimal.o

# lines

lines : ${OBJDIR}/lines.o  ${LIBEIBASE} ${LIBEI}
	${LINK} -o lines ${OBJDIR}/lines.o ${LIBEI} ${LIBS}

${OBJDIR}/lines.o : ${TESTS}/lines.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/lines.c -o ${OBJDIR}/lines.o

# frame

frame : ${OBJDIR}/frame.o  ${LIBEIBASE} ${LIBEI}
	${LINK} -o frame ${LDFLAGS} ${OBJDIR}/frame.o  ${LIBEI} ${LIBS}

${OBJDIR}/frame.o : ${TESTS}/frame.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/frame.c -o ${OBJDIR}/frame.o

# toplevel

toplevel : ${OBJDIR}/toplevel.o  ${LIBEIBASE} ${LIBEI}
	${LINK} -o toplevel ${LDFLAGS} ${OBJDIR}/toplevel.o  ${LIBEI} ${LIBS}

${OBJDIR}/toplevel.o : ${TESTS}/toplevel.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/toplevel.c -o ${OBJDIR}/toplevel.o

# test_button

test_button : ${OBJDIR}/test_button.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o test_button ${LDFLAGS} ${OBJDIR}/test_button.o ${LIBEI} ${LIBS}

${OBJDIR}/test_button.o : ${TESTS}/test_button.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/test_button.c -o ${OBJDIR}/test_button.o

# button

button : ${OBJDIR}/button.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o button ${LDFLAGS} ${OBJDIR}/button.o ${LIBEI} ${LIBS}

${OBJDIR}/button.o : ${TESTS}/button.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/button.c -o ${OBJDIR}/button.o

# hello_world

hello_world : ${OBJDIR}/hello_world.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o hello_world ${LDFLAGS} ${OBJDIR}/hello_world.o ${LIBEI} ${LIBS}

${OBJDIR}/hello_world.o : ${TESTS}/hello_world.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/hello_world.c -o ${OBJDIR}/hello_world.o

# puzzle

puzzle : ${OBJDIR}/puzzle.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o puzzle ${LDFLAGS} ${OBJDIR}/puzzle.o ${LIBEI} ${LIBS}

${OBJDIR}/puzzle.o : ${TESTS}/puzzle.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/puzzle.c -o ${OBJDIR}/puzzle.o


# two048

two048 : ${OBJDIR}/two048.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o two048 ${LDFLAGS} ${OBJDIR}/two048.o ${LIBEI} ${LIBS}

${OBJDIR}/two048.o : ${TESTS}/two048.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/two048.c -o ${OBJDIR}/two048.o



# Building of the library libei

${LIBEI} : ${LIBEIOBJS}
	ar rcs ${LIBEI} ${LIBEIOBJS}



# Building of the doxygen documentation.

doc :
	doxygen docs/doxygen.cfg



# Removing all built files.

clean:
	rm -f ${TARGETS}
	rm -f *.exe
	rm -f ${OBJDIR}/*
