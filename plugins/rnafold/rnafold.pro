#! [0]
TEMPLATE      = lib
CONFIG       += plugin static
QT           += widgets
INCLUDEPATH  += ../../app
HEADERS       = rnafold.h
SOURCES       = rnafold.cpp
TARGET        = $$qtLibraryTarget(rna_rnafold)
DESTDIR       = ../../plugins
#! [0]
