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

include($$PWD/global.pri)

TEMPLATE = subdirs

SUBDIRS = libraries/libtide \
          thirdparty/Hmdf \
          libraries/libmetocean

!equals(GUI_DISABLE,1) {
  SUBDIRS  += MetOceanViewer2
}

CONFIG += ordered           

HEADERS += version.h

unix {
  #SUBDIRS+=ProcessCrmsDatabase
}
