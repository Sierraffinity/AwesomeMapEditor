#
# AwesomeMapEditor: A map editor for GBA Pokémon games.
# Copyright (C) 2016 Diegoisawesome, Pokedude
# License: General Public License 3.0
#


#
# QMake Settings, 1
#
QT         += core gui widgets
TARGET      = AwesomeMapEditor
TEMPLATE    = app
CONFIG     += c++11
DEFINES    += AME_EDITOR

# do only use if getting those useless BUILT_IN warnings
CONFIG     += warn_off


#
# QMake Settings, 2
#
VERSION = 1.0
win:QMAKE_TARGET_COMPANY     = DoMoreAwesome
win:QMAKE_TARGET_PRODUCT     = AwesomeMapEditor
win:QMAKE_TARGET_DESCRIPTION = A map editor for GBA Pokémon games.
win:QMAKE_TARGET_COPYRIGHT   = (C) 2016-2017 Diegoisawesome, Pokedude


#
# QMake Settings, 3
#
INCLUDEPATH += include
INCLUDEPATH += include/AME/Forms
INCLUDEPATH += include/AME/Widgets
#INCLUDEPATH += include/AME/Widgets/OpenGL
INCLUDEPATH += include/AME/Widgets/Rendering
INCLUDEPATH += $$PWD/../QBoy/include
DEPENDPATH  += $$PWD/../QBoy/include
INCLUDEPATH += $$PWD/../QBoy/bin
DEPENDPATH += $$PWD/../QBoy/bin
INCLUDEPATH += $$PWD/../yaml-cpp/include
DEPENDPATH += $$PWD/../yaml-cpp/include
INCLUDEPATH += $$PWD/../yaml-cpp/bin
DEPENDPATH += $$PWD/../yaml-cpp/bin
QMAKE_LFLAGS += -static-libgcc -static-libstdc++
#win32: LIBS += -lopengl32
#unix:  LIBS += -lGL


#
# QMake Settings, 4
#
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../QBoy/bin/release/ -lQBoy
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../QBoy/bin/debug/ -lQBoy
else:unix: LIBS += -L$$PWD/../QBoy/bin/ -lQBoy
unix|win32: LIBS += -L$$PWD/../yaml-cpp/bin/ -lyaml-cpp


#
# Source Files
#
SOURCES += \
    src/main.cpp \
    src/Forms/MainWindow.cpp \
    src/System/WriteEntry.cpp \
    src/System/ErrorStack.cpp \
    src/Text/Tables.cpp \
    src/Text/String.cpp \
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
    src/Mapping/MapNameTable.cpp \
    src/Mapping/Map.cpp \
    src/Mapping/MapBank.cpp \
    src/Widgets/QHexLineEdit.cpp \
    src/Widgets/QHexSpinBox.cpp \
    src/Mapping/MapBankTable.cpp \
    src/System/Configuration.cpp \
    src/System/LoadedData.cpp \
    src/Widgets/Misc/Messages.cpp \
    #src/Widgets/OpenGL/AMEMapView.cpp \
    #src/Widgets/OpenGL/AMEBlockView.cpp \
    src/Widgets/AMEImageButton.cpp \
    src/Forms/ErrorWindow.cpp \
    #src/Widgets/OpenGL/AMEBorderView.cpp \
    src/Structures/PokemonTable.cpp \
    src/Graphics/OverworldTable.cpp \
    #src/Widgets/OpenGL/AMEEntityView.cpp \
    src/Graphics/PropertyTable.cpp \
    src/System/Settings.cpp \
    src/Entities/EntityRawData.cpp \
    src/Mapping/MapLayoutTable.cpp \
    src/Widgets/QFilterChildrenProxyModel.cpp \
    src/Structures/ItemTable.cpp \
    src/Forms/SettingsDialog.cpp \
    src/Forms/TilesetDialog.cpp \
    src/Algorithm/PaletteQuantizer.cpp \
    src/Widgets/Listeners/MovePermissionListener.cpp \
    src/Widgets/Chunks.cpp \
    src/Widgets/QHexEdit.cpp \
    src/Widgets/Commands.cpp \
    #src/Widgets/OpenGL/AMEOpenGLShared.cpp
    src/Widgets/Rendering/AMEBlockView.cpp \
    src/Widgets/Rendering/AMEBorderView.cpp \
    src/Widgets/Rendering/AMEEntityView.cpp \
    src/Widgets/Rendering/AMEMapView.cpp \
    src/Widgets/Rendering/Cursor.cpp \
    src/Mapping/CurrentMapManager.cpp


#
# Header Files
#
HEADERS  += \
    include/AME/Forms/MainWindow.h \
    include/AME/Structures/WildPokemonTable.hpp \
    include/AME/System/ErrorStack.hpp \
    include/AME/System/WriteEntry.hpp \
    include/AME/System/IUndoable.hpp \
    include/AME/Text/String.hpp \
    include/AME/Text/Tables.hpp \
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
    include/AME/Mapping/Map.hpp \
    include/AME/Mapping/MapBank.hpp \
    include/AME/Widgets/QHexLineEdit.h \
    include/AME/Widgets/QHexSpinBox.h \
    include/AME/Mapping/MapBankTable.hpp \
    include/AME/Mapping/MapNameTable.hpp \
    include/AME/System/Configuration.hpp \
    include/AME/System/LoadedData.hpp \
    include/AME/Widgets/Misc/Messages.hpp \
    #include/AME/Widgets/OpenGL/AMEMapView.h \
    #include/AME/Widgets/OpenGL/AMEBlockView.h \
    include/AME/Widgets/AMEImageButton.h \
    include/AME/Forms/ErrorWindow.h \
    #include/AME/Widgets/OpenGL/AMEBorderView.h \
    include/AME/Structures/PokemonTable.hpp \
    include/AME/Graphics/OverworldTable.hpp \
    #include/AME/Widgets/OpenGL/AMEEntityView.h \
    include/AME/Graphics/PropertyTable.hpp \
    include/AME/Graphics/Property.hpp \
    include/AME/System/Settings.hpp \
    include/AME/Mapping/MapName.hpp \
    include/AME/Mapping/MapLayoutTable.hpp \
    include/AME/Widgets/QFilterChildrenProxyModel.h \
    include/AME/Structures/ItemTable.hpp \
    include/AME/Forms/SettingsDialog.h \
    include/AME/Forms/TilesetDialog.h \
    include/AME/Algorithm/PaletteQuantizer.hpp \
    include/AME/Widgets/Chunks.h \
    include/AME/Widgets/Commands.h \
    include/AME/Widgets/QHexEdit.h \
    #include/AME/Widgets/OpenGL/AMEOpenGLShared.hpp
    include/AME/Widgets/Rendering/AMEBlockView.h \
    include/AME/Widgets/Rendering/AMEBorderView.h \
    include/AME/Widgets/Rendering/AMEEntityView.h \
    include/AME/Widgets/Rendering/AMEMapView.h \
    include/AME/Widgets/Listeners/MovePermissionListener.h \
    include/AME/Widgets/Rendering/Cursor.hpp \
    include/AME/Mapping/CurrentMapManager.hpp


#
# Forms
#
FORMS    += resources/UI/MainWindow.ui \
    resources/UI/ErrorWindow.ui \
    resources/UI/SettingsDialog.ui \
    resources/UI/TilesetDialog.ui

#
# Resources
#
RESOURCES += \
    resources/images.qrc #\
    #resources/shaders.qrc

DISTFILES += \
    resources/config/BPRE.yaml \
    #resources/shaders/NormalFragmentShader \
    #resources/shaders/PrimitiveVertexShader.glsl \
    #resources/shaders/PrimitiveFragmentShader.glsl
