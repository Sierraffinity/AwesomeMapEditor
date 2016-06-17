#
# AwesomeMapEditor: A map editor for GBA Pokémon games.
# Copyright (C) 2016 Diegoisawesome, Pokedude
# License: General Public License 3.0
#


#
# QMake Settings, 1
#
QT         += core gui opengl widgets
TARGET      = AwesomeMapEditor
TEMPLATE    = app
CONFIG     += c++11
DEFINES    += AME_EDITOR


#
# QMake Settings, 2
#
VERSION = 1.0
win:QMAKE_TARGET_COMPANY     = DoMoreAwesome
win:QMAKE_TARGET_PRODUCT     = AwesomeMapEditor
win:QMAKE_TARGET_DESCRIPTION = A map editor for GBA Pokémon games.
win:QMAKE_TARGET_COPYRIGHT   = (C) 2016 Diegoisawesome, Pokedude


#
# QMake Settings, 3
#
INCLUDEPATH += include
INCLUDEPATH += include/AME/Forms
INCLUDEPATH += include/AME/Widgets
INCLUDEPATH += $$PWD/../QBoy/include
DEPENDPATH  += $$PWD/../QBoy/include
INCLUDEPATH += $$PWD/../yaml-cpp/include
DEPENDPATH += $$PWD/../yaml-cpp/include
QMAKE_LFLAGS += -static-libgcc -static-libstdc++


#
# QMake Settings, 4
#
unix|win32: LIBS += -L$$PWD/../QBoy/bin/debug/ -lQBoy
unix|win32: LIBS += -L$$PWD/../yaml-cpp/bin/debug/ -lyaml-cpp


#
# Source Files
#
SOURCES += \
    src/main.cpp \
    src/Forms/MainWindow.cpp \
    src/System/WriteEntry.cpp \
    src/System/ErrorStack.cpp \
    src/Structures/WildPokemonTable.cpp \
    src/Structures/WildPokemonSubTable.cpp \
    src/Structures/WildPokemonArea.cpp \
    src/Entities/MapScriptTable.cpp \
    src/Entities/ConnectionTable.cpp \
    src/Entities/EventTable.cpp \
    src/Graphics/Tileset.cpp \
    src/Graphics/TilesetManager.cpp \
    src/Mapping/MapBorder.cpp \
    src/Mapping/MapHeader.cpp \
    src/Widgets/QToolButtonLastUsed.cpp \
    src/Mapping/Map.cpp \
    src/Widgets/qhexedit.cpp \
    src/Widgets/chunks.cpp \
    src/Widgets/commands.cpp \
    src/Mapping/MapBank.cpp \
    src/Widgets/QHexLineEdit.cpp \
    src/Mapping/MapBankTable.cpp \
    src/System/Configuration.cpp \
    src/System/LoadedData.cpp \
    src/Widgets/Misc/Messages.cpp


#
# Header Files
#
HEADERS  += \
    include/AME/Forms/MainWindow.h \
    include/AME/Structures/WildPokemonTable.hpp \
    include/AME/System/ErrorStack.hpp \
    include/AME/System/WriteEntry.hpp \
    include/AME/System/IUndoable.hpp \
    include/AME/Structures/WildPokemonSubTable.hpp \
    include/AME/Structures/StructureErrors.hpp \
    include/AME/Structures/WildPokemonArea.hpp \
    include/AME/Structures/WildPokemonEncounter.hpp \
    include/AME/Entities/Warp.hpp \
    include/AME/Entities/Sign.hpp \
    include/AME/Entities/EntityTypes.hpp \
    include/AME/Entities/Trigger.hpp \
    include/AME/Entities/Npc.hpp \
    include/AME/Entities/Connection.hpp \
    include/AME/Entities/MapScript.hpp \
    include/AME/Entities/AutoScript.hpp \
    include/AME/Entities/Tables/EntityErrors.hpp \
    include/AME/Entities/Tables/MapScriptTable.hpp \
    include/AME/Entities/Tables/ConnectionTable.hpp \
    include/AME/Entities/Tables/EventTable.hpp \
    include/AME/Graphics/Tileset.hpp \
    include/AME/Graphics/Tile.hpp \
    include/AME/Graphics/Block.hpp \
    include/AME/Graphics/GraphicsErrors.hpp \
    include/AME/Mapping/MapBlock.hpp \
    include/AME/Mapping/MapHeader.hpp \
    include/AME/Graphics/TilesetManager.hpp \
    include/AME/Mapping/MapBorder.hpp \
    include/AME/Mapping/MappingErrors.hpp \
    include/AME/Widgets/QToolButtonLastUsed.h \
    include/AME/Mapping/Map.hpp \
    include/AME/Mapping/MapBank.hpp \
    include/AME/Widgets/qhexedit.h \
    include/AME/Widgets/chunks.h \
    include/AME/Widgets/commands.h \
    include/AME/Widgets/QHexLineEdit.h \
    include/AME/Mapping/MapBankTable.hpp \
    include/AME/System/Configuration.hpp \
    include/AME/System/LoadedData.hpp \
    include/AME/Widgets/Misc/Messages.hpp


#
# Forms
#
FORMS    += resources/UI/MainWindow.ui

#
# Resources
#
RESOURCES += \
    resources/images.qrc

DISTFILES += \
    resources/config/BPRE.yaml
