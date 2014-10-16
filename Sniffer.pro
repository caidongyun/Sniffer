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

RESOURCES = res/appliction.qrc

# Input
LIBS += -lpcap

UI_DIR += ui

FORMS += ui/interfacesselect.ui

SOURCES += src/main.cpp\
        src/sniffer.cpp\
        src/snifferthread.cpp\
        src/analysisthread.cpp\
        src/prototree.cpp\
        src/mytableview.cpp\
        src/mainwindow.cpp\
        src/originpacket_text.cpp\
        src/filterlineedit.cpp\
        src/treeitem.cpp

HEADERS  += include/sniffer.h\
         include/snifferthread.h\
         include/analysisthread.h\
         include/prototree.h\
         include/snifferutil.h\
         include/mytableview.h\
         include/mainwindow.h\
         include/originpacket_text.h\
         include/filterlineedit.h\
         include/treeitem.h

