add_qtc_library(appexp
  DEPENDS Qt5::Widgets Qt5::Core Qt5::Gui Utils
  SOURCES
    main.cpp
    ../tools/qtcreatorcrashhandler/crashhandlersetup.cpp ../tools/qtcreatorcrashhandler/crashhandlersetup.h
)
