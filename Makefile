DIST := build
SOURCE := src
INCS := include

all: dist liblspe.a liblsped.a example test

dist:
	-mkdir ${DIST}

liblspe.a:
	-cd ${SOURCE} && make liblspe.a
	-mv -f ${SOURCE}/$@ ${DIST}

liblsped.a:
	-cd ${SOURCE} && make liblsped.a
	-mv -f ${SOURCE}/$@ ${DIST}

example:
	-cd example && make all

test:
	-cd test && make all

.PHONY: clean
clean:  
	-cd src && make clean
	-cd example && make clean
	-cd test && make clean
