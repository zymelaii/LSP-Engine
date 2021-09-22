.SUFFIXES: .cpp .c

HOME := ..

vpath %.c ${HOME}/src
vpath %.cpp ${HOME}/src
vpath %.h ${HOME}/include/lspe
vpath %.hpp ${HOME}/include/lspe

INCS := -I${HOME}/include
LIBS :=
DIST := ${HOME}/build

# CFLAGS := -O0 -pipe -fno-keep-inline-dllexport -fexceptions
CFLAGS :=

OBJECTS := vec.obj mat.obj \
	bbox.obj abt.obj broadphase.obj \
	shape_line.obj shape_circle.obj shape_polygen.obj shape_ellipse.obj shape_bezier.obj \
	collision.obj

DOBJECTS := vec.dobj mat.dobj \
	bbox.dobj abt.dobj broadphase.dobj \
	shape_line.dobj shape_circle.dobj shape_polygen.dobj shape_ellipse.dobj shape_bezier.dobj \
	collision.dobj

%.obj: %.cpp
	gcc ${INCS} ${LIBS} ${CFLAGS} -c -o $@ $<

%.dobj: %.cpp
	gcc ${INCS} ${LIBS} ${CFLAGS} -DDEBUG -c -o $@ $<

liblspe.a: ${OBJECTS}
	ar -crs $@ $^

liblsped.a: ${DOBJECTS}
	ar -crs $@ $^

all: liblspe.a  liblsped.a

.PHONY: clean
clean:
	-rm -f *.obj *.dobj *.o *.a *.gch *.exe *.dll
