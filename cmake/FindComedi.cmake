#############################################################################
#
# This file is part of the ViSP software.
# Copyright (C) 2005 - 2015 by Inria. All rights reserved.
#
# This software is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# ("GPL") version 2 as published by the Free Software Foundation.
# See the file LICENSE.txt at the root directory of this source
# distribution for additional information about the GNU GPL.
#
# For using ViSP with software that can not be combined with the GNU
# GPL, please contact Inria about acquiring a ViSP Professional
# Edition License.
#
# See http://visp.inria.fr for more information.
#
# This software was developed at:
# Inria Rennes - Bretagne Atlantique
# Campus Universitaire de Beaulieu
# 35042 Rennes Cedex
# France
#
# If you have questions regarding the use of this file, please contact
# Inria at visp@inria.fr
#
# This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
# WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
#
# Description:
# Try to find comedi library
#
# COMEDI_FOUND
# COMEDI_INCLUDE_DIRS
# COMEDI_LIBRARIES
#
# Authors:
# Fabien Spindler
#
#############################################################################

find_path(COMEDI_INCLUDE_DIRS comedilib.h
  PATHS
    $ENV{COMEDI_HOME}/include
    /usr/include
    /usr/local/include
)

find_library(COMEDI_LIBRARIES
  NAMES comedi
  PATHS 
    $ENV{COMEDI_HOME}/lib
    /usr/lib
    /usr/local/lib
)

if(COMEDI_LIBRARIES AND COMEDI_INCLUDE_DIRS)
  set(COMEDI_FOUND TRUE)
else()
  set(COMEDI_FOUND FALSE)
endif()
  
mark_as_advanced(
  COMEDI_INCLUDE_DIRS
  COMEDI_LIBRARIES
)
