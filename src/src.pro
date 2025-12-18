TEMPLATE = lib

CONFIG += link_pkgconfig plugin
CONFIG -= qt

test {
	CONFIG += staticlib
}

TARGET = remote-images
INCLUDEPATH += . 

QMAKE_CXXFLAGS += -fPIC 

PKGCONFIG += glib-2.0
PKGCONFIG += gtk+-3.0

EDS_VERSION = $$system(pkg-config --modversion libedataserver-1.2)

message("EDS Version: $$EDS_VERSION")

PKGCONFIG += evolution-mail-3.0

DEFINES += GETTEXT_PACKAGE='\\"remote-images\\"'
DESTDIR = ../lib

target2.path = $$(HOME)/.local/share/evolution/modules/lib/evolution/plugins/
target2.files += remote-images.eplug

target.path = $$(HOME)/.local/share/evolution/modules/lib/evolution/modules/
target.files = $$DESTDIR/lib$${TARGET}.so 

INSTALLS += target 
INSTALLS += target2

debug {
	OBJECTS_DIR = .obj.debug
}

release {
	OBJECTS_DIR = .obj.release
}

HEADERS += remote-images-extension-gtk3.h 
SOURCES += remote-images-extension-gtk3.c 

