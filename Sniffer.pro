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

# Input
LIBS += -lpcap

SOURCES += src/main1.cpp\
        src/sniffer.cpp\
        src/snifferthread.cpp\
        src/mainwindow.cpp

HEADERS  += include/mainwindow.h\
         include/sniffer.h\
         include/snifferutil.h\
         include/snifferthread.h

