###############################################################################
#                                                                             #
# The MIT License                                                             #
#                                                                             #
# Copyright (C) 2017 by Juergen Skrotzky (JorgenVikingGod@gmail.com)          #
#               >> https://github.com/Jorgen-VikingGod                        #
#                                                                             #
# Sources: https://github.com/Jorgen-VikingGod/Qt-Frameless-Window-DarkStyle  #
#                                                                             #
###############################################################################

INCLUDEPATH += $$PWD/framelesswindow


SOURCES     += $$PWD/framelesswindow/framelesswindow.cpp \
               $$PWD/framelesswindow/windowdragger.cpp \
               $$PWD/DarkStyle.cpp


HEADERS     += $$PWD/framelesswindow/framelesswindow.h \
               $$PWD/framelesswindow/windowdragger.h \
               $$PWD/DarkStyle.h


FORMS       += $$PWD/framelesswindow/framelesswindow.ui

RESOURCES   += $$PWD/darkstyle.qrc \
               $$PWD/framelesswindow.qrc
