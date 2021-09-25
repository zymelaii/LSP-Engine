HOME := ../..
SOURCE := main.cpp mainwidget.h mainwidget.cpp mainwidget.ui \
	lspecanvas.h lspecanvas.cpp lspecanvas.ui stable.h

vpath %.a ${HOME}/build

all: debug release

debug: liblsped.a ${SOURCE} make-debug.yml
	@qtcc make-debug.yml

release: liblspe.a ${SOURCE} make-release.yml
	@qtcc make-release.yml

.PHONY: clean
clean:
	-rm -rf Debug Release