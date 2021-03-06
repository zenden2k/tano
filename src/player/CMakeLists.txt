#############################################################################
# Tano - An Open IP TV Player
# Copyright (C) 2013 Tadej Novak <tadej@tano.si>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>./
#############################################################################

IF(MINGW OR MSVC)
    CONFIGURE_FILE(
        ${CMAKE_SOURCE_DIR}/src/player/tano.rc.in
        ${CMAKE_BINARY_DIR}/src/player/tano.rc
    )
ENDIF(MINGW OR MSVC)

SET(Tano_Desktop
    ${CMAKE_SOURCE_DIR}/src/player/tano.desktop
)

SET(Tano_Logo
    ${CMAKE_SOURCE_DIR}/src/player/tano.png
)

# Define the QRC source files used by Tano
SET(Tano_Resrcs
    ../../data/logo/logo.qrc
)

SET(Tano_Srcs
    main.cpp
)

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    SET(Tano_Iconset ${CMAKE_SOURCE_DIR}/osx/Tano.iconset)

    FILE(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/src/player/${PROJECT_NAME}.app/Contents/Resources)
    FIND_PROGRAM(ICONUTIL_EXECUTABLE NAMES iconutil)
    ADD_CUSTOM_COMMAND(
        OUTPUT ${CMAKE_BINARY_DIR}/src/player/${PROJECT_NAME}.app/Contents/Resources/${PROJECT_NAME}.icns
        COMMAND ${ICONUTIL_EXECUTABLE} --convert icns --output ${CMAKE_BINARY_DIR}/src/player/${PROJECT_NAME}.app/Contents/Resources/${PROJECT_NAME}.icns
        ${Tano_Iconset}
    )
    SET(Tano_Srcs ${Tano_Srcs} ${CMAKE_BINARY_DIR}/src/player/${PROJECT_NAME}.app/Contents/Resources/${PROJECT_NAME}.icns)
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")


###############
# Qt settings #
###############
# This script runs the resources compiler rcc
IF(QT5)
    QT5_ADD_RESOURCES(Tano_Resrcs_Srcs ${Tano_Resrcs})
ELSE(QT5)
    QT4_ADD_RESOURCES(Tano_Resrcs_Srcs ${Tano_Resrcs})
ENDIF(QT5)


#####################
# Compiler settings #
#####################
# This needs to be added in order for #include to work inside .cpp files
INCLUDE_DIRECTORIES(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/core
    ${CMAKE_SOURCE_DIR}/src/widgets
    ${CMAKE_BINARY_DIR}
    ${CMAKE_BINARY_DIR}/src
)

# Include additional libraries
INCLUDE_DIRECTORIES(${LIBVLCQT_INCLUDE_DIR})

# Compile resources with windres
IF(MINGW)
    ADD_CUSTOM_COMMAND(
        OUTPUT ${CMAKE_BINARY_DIR}/src/player/tano.obj
        COMMAND ${CMAKE_RC_COMPILER}
        -i ${CMAKE_BINARY_DIR}/src/player/tano.rc
        -o ${CMAKE_BINARY_DIR}/src/player/tano.obj
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/src/player"
    )
    SET(Tano_Srcs ${Tano_Srcs} ${CMAKE_BINARY_DIR}/src/player/tano.obj)
ENDIF(MINGW)

IF(MSVC)
    SET(Tano_Srcs ${Tano_Srcs} ${CMAKE_BINARY_DIR}/src/player/tano.rc)
ENDIF(MSVC)

# This line tells cmake to create the Tano program.
ADD_EXECUTABLE(${PROJECT_NAME} MACOSX_BUNDLE WIN32 ${Tano_Srcs} ${Tano_Resrcs_Srcs})
ADD_DEPENDENCIES(${PROJECT_NAME} ${TANO_CORE} ${TANO_WIDGETS} ${TANO_CONFIG})

# MSVC: Avoid Debug/Release/... subfolders
IF(MSVC)
    SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ".")
    SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_DEBUG ".")
    SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_RELEASE ".")
    SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_RELWITHDUBUGINFO ".")
    SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL ".")
    SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_PROFILE ".")
ENDIF(MSVC)

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
   SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES MACOSX_BUNDLE_INFO_PLIST "${CMAKE_BINARY_DIR}/osx/Info.plist")
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
   SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${PROJECT_NAME_LOWER})
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

# Here the needed Libraries are added to the link command.
IF(STATIC)
    TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${LIBVLC_LIBRARY} ${LIBVLCCORE_LIBRARY})
    TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${LIBVLCQT_LIBRARY} ${LIBVLCQT_WIDGETS_LIBRARY})
ENDIF(STATIC)

IF(QT5)
    QT5_USE_MODULES(${PROJECT_NAME} Concurrent Gui Network PrintSupport Sql Widgets Xml)
    TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${TANO_CORE_LIB} ${TANO_WIDGETS_LIB} ${TANO_CONFIG_LIB})
ELSE(QT5)
    TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${TANO_CONFIG_LIB} ${TANO_CORE_LIB} ${TANO_WIDGETS_LIB} ${QT_LIBRARIES})
ENDIF(QT5)

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin" AND NOT STATIC)
    ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD COMMAND
        ${CMAKE_INSTALL_NAME_TOOL} -change ${TANO_CORE_LIB} "@loader_path/lib/${LIBSTART}${TANO_CORE}.${LIBEND}" $<TARGET_FILE:${PROJECT_NAME}>)
    ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD COMMAND
        ${CMAKE_INSTALL_NAME_TOOL} -change ${TANO_WIDGETS_LIB} "@loader_path/lib/${LIBSTART}${TANO_WIDGETS}.${LIBEND}" $<TARGET_FILE:${PROJECT_NAME}>)
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin" AND NOT STATIC)


################
# Installation #
################
# Install tano exe
INSTALL(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/${TANO_INSTALL_BIN_DIR}
    BUNDLE DESTINATION .
)

IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    INSTALL(FILES ${Tano_Desktop} DESTINATION "${CMAKE_INSTALL_PREFIX}/share/applications")
    INSTALL(FILES ${Tano_Logo} DESTINATION "${CMAKE_INSTALL_PREFIX}/share/icons")
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
