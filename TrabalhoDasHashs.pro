TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp

HEADERS += \
    funcoes_hash.h \
    hash_encadeada.h \
    hash_enderecamento_aberto.h \
    ler_arquivo.h

DISTFILES += \
    teste.txt
