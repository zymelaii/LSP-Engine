HOME := ../..
SOURCE := main.cpp mainwidget.h mainwidget.cpp mainwidget.ui \
	lspecanvas.h lspecanvas.cpp lspecanvas.ui stable.h

vpath %.a ${HOME}/build

all: debug release

debug: liblsped.a ${SOURCE} make-debug.yml
	qtcc make-debug.yml

release: liblspe.a ${SOURCE} make-release.yml
	qtcc make-realease.yml