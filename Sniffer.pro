#-------------------------------------------------
#
#        Filename: Sniffer.pro
#
#     Description: The project file for Sniffer 
#
#          Author: Zengping(zengping@ncic.ac.cn)
#
#         Created: 2014-09-17
#
#-------------------------------------------------

TARGET = Sniffer
TEMPLATE = app
INCLUDEPATH += .
DEPENDPATH += .

# Input
LIBS += -lpcap


UI_DIR += ui

SOURCES += src/main.cpp\
        src/sniffer.cpp\
        src/snifferthread.cpp\
        src/mytableview.cpp\
        src/mainwindow.cpp\
        src/originpacket_text.cpp

HEADERS  += include/sniffer.h\
         include/snifferthread.h\
         include/snifferutil.h\
         include/mytableview.h\
         include/mainwindow.h\
         include/originpacket_text.h

