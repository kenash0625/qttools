TEMPLATE = app
CONFIG -= qt app_bundle
CONFIG += console
SOURCES += main.cpp

isEmpty(BOOST_INCLUDE_DIR):BOOST_INCLUDE_DIR=$$(BOOST_INCLUDE_DIR)
!isEmpty(BOOST_INCLUDE_DIR): INCLUDEPATH *= $$BOOST_INCLUDE_DIR
