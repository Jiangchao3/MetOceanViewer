#-------------------------------GPL-------------------------------------#
#
# MetOcean Viewer - A simple interface for viewing hydrodynamic model data
# Copyright (C) 2019  Zach Cobell
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
#-----------------------------------------------------------------------#
QT += network positioning
QT -= gui

include($$PWD/../global.pri)

CONFIG += c++11 console
CONFIG -= app_bundle
TARGET = MetOceanData

INCLUDEPATH += $$PWD/../thirdparty/boost_1_67_0

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
    metoceandata.cpp \
    options.cpp

INCLUDEPATH += ../

HEADERS += \
    metoceandata.h \
    options.h \
    optionslist.h

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../libraries/libmetocean/release/ -lmetocean
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../libraries/libmetocean/debug/ -lmetocean
else:unix: LIBS += -L$$OUT_PWD/../libraries/libmetocean/ -lmetocean

INCLUDEPATH += $$PWD/../libraries/libmetocean
DEPENDPATH += $$PWD/../libraries/libmetocean

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../libraries/libmetocean/release/libmetocean.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../libraries/libmetocean/debug/libmetocean.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../libraries/libmetocean/release/metocean.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../libraries/libmetocean/debug/metocean.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../libraries/libmetocean/libmetocean.a

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../thirdparty/ezproj/src/release/ -lezproj
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../thirdparty/ezproj/src/debug/ -lezproj
else:unix: LIBS += -L$$OUT_PWD/../thirdparty/ezproj/src/ -lezproj

INCLUDEPATH += $$PWD/../thirdparty/ezproj/src
DEPENDPATH += $$PWD/../thirdparty/ezproj/src

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../thirdparty/ezproj/src/release/libezproj.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../thirdparty/ezproj/src/debug/libezproj.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../thirdparty/ezproj/src/release/ezproj.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../thirdparty/ezproj/src/debug/ezproj.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../thirdparty/ezproj/src/libezproj.a


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../libraries/libtide/release/ -ltide
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../libraries/libtide/debug/ -ltide
else:unix: LIBS += -L$$OUT_PWD/../libraries/libtide/ -ltide

INCLUDEPATH += $$PWD/../libraries/libtide
DEPENDPATH += $$PWD/../libraries/libtide

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../libraries/libtide/release/libtide.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../libraries/libtide/debug/libtide.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../libraries/libtide/release/tide.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../libraries/libtide/debug/tide.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../libraries/libtide/libtide.a

LIBS += -lnetcdf
