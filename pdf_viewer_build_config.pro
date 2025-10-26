QMAKE_MKSPECS = /opt/homebrew/opt/qt@6/lib/qt/mkspecs
TEMPLATE = app
TARGET = opusreader
VERSION = 2.0.0
INCLUDEPATH += ./pdf_viewer

QT += core gui widgets opengl openglwidgets network 3dinput dbus

CONFIG += c++17

DEFINES += OPUSREADER_QT6
DEFINES += QT_3DINPUT_LIB QT_OPENGL_LIB QT_WIDGETS_LIB

MUPDF_CFLAGS = $$system(mupdf-config --cflags 2> /dev/null)
isEmpty(MUPDF_CFLAGS) {
    MUPDF_PREFIX = $$system(brew --prefix mupdf 2> /dev/null)
    isEmpty(MUPDF_PREFIX) {
        MUPDF_PREFIX = /opt/homebrew
    }
    INCLUDEPATH += $$MUPDF_PREFIX/include
} else {
    QMAKE_CXXFLAGS += $$MUPDF_CFLAGS
}

MUPDF_LIBS = $$system(mupdf-config --libs 2> /dev/null)
isEmpty(MUPDF_LIBS) {
    isEmpty(MUPDF_PREFIX) {
        MUPDF_PREFIX = $$system(brew --prefix mupdf 2> /dev/null)
        isEmpty(MUPDF_PREFIX) {
            MUPDF_PREFIX = /opt/homebrew
        }
    }
    LIBS += -L$$MUPDF_PREFIX/lib -L/opt/homebrew/lib -lmupdf -lmupdf-third -lz -lharfbuzz -lfreetype -ljbig2dec -ljpeg -lopenjp2 -lmujs -lgumbo
} else {
    LIBS += $$MUPDF_LIBS
}

CONFIG(non_portable){
    DEFINES += NON_PORTABLE
}

# Build directories - keep intermediate files organized
OBJECTS_DIR = builds/intermediate/obj
MOC_DIR = builds/intermediate/moc
RCC_DIR = builds/intermediate/rcc
UI_DIR = builds/intermediate/ui
DESTDIR = builds

# Input
HEADERS += pdf_viewer/book.h \
           pdf_viewer/config.h \
           pdf_viewer/database.h \
           pdf_viewer/document.h \
           pdf_viewer/document_view.h \
           pdf_viewer/fts_fuzzy_match.h \
           pdf_viewer/rapidfuzz_amalgamated.hpp \
           pdf_viewer/input.h \
           pdf_viewer/main_widget.h \
           pdf_viewer/pdf_renderer.h \
           pdf_viewer/pdf_view_opengl_widget.h \
           pdf_viewer/checksum.h \
           pdf_viewer/new_file_checker.h \
           pdf_viewer/coordinates.h \
           pdf_viewer/sqlite3.h \
           pdf_viewer/sqlite3ext.h \
           pdf_viewer/ui.h \
           pdf_viewer/path.h \
           pdf_viewer/utf8.h \
           pdf_viewer/utils.h \
           pdf_viewer/utf8/checked.h \
           pdf_viewer/utf8/core.h \
           pdf_viewer/utf8/unchecked.h \
           pdf_viewer/synctex/synctex_parser.h \
           pdf_viewer/synctex/synctex_parser_utils.h \
           pdf_viewer/RunGuard.h \
           pdf_viewer/OpenWithApplication.h

SOURCES += pdf_viewer/book.cpp \
           pdf_viewer/config.cpp \
           pdf_viewer/database.cpp \
           pdf_viewer/document.cpp \
           pdf_viewer/document_view.cpp \
           pdf_viewer/input.cpp \
           pdf_viewer/main.cpp \
           pdf_viewer/main_widget.cpp \
           pdf_viewer/pdf_renderer.cpp \
           pdf_viewer/pdf_view_opengl_widget.cpp \
           pdf_viewer/checksum.cpp \
           pdf_viewer/new_file_checker.cpp \
           pdf_viewer/coordinates.cpp \
           pdf_viewer/sqlite3.c \
           pdf_viewer/ui.cpp \
           pdf_viewer/path.cpp \
           pdf_viewer/utils.cpp \
           pdf_viewer/synctex/synctex_parser.c \
           pdf_viewer/synctex/synctex_parser_utils.c \
           pdf_viewer/RunGuard.cpp \
           pdf_viewer/OpenWithApplication.cpp


mac {
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 15.0
    CONFIG += sdk_no_version_check
    QMAKE_CXXFLAGS += -std=c++17
    CONFIG(cli_no_bundle) {
        CONFIG -= app_bundle
    } else {
        ICON = resources/icon.icns
        QMAKE_INFO_PLIST = resources/Info.plist
    }
}
