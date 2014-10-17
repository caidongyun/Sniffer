/*
 * =====================================================================================
 *
 *       Filename:  mainwindow.h
 *
 *    Description:  主窗口实现文件头文件
 *
 *        Version:  1.0
 *        Created:  2014年09月21日 20时16分19秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ping Zeng (zp), zengping@ncic.ac.cn
 *   Organization:  HPC Research Center
 *
 * =====================================================================================
 */
#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include <QMainWindow>
#include <QMessageBox>
#include <QTextBlock>
#include <QSplitter>
#include <QTextEdit>
#include <QAction>
#include <QIcon>
#include <QToolBar>
#include <QMenuBar>
#include <QMenu>
#include <QKeySequence>

#include "sniffer.h"
#include "snifferthread.h"
#include "sniffertypes.h"
#include "snifferutil.h"
#include "mytableview.h"
#include "originpacket_text.h"
#include "prototree.h"
#include "filterlineedit.h"
#include "interfacesselectdialog.h"

class SnifferThread;
class MyTableView;
class QMessageBox;
class QTreeWidget;
class QTableView;
class QTextEdit;
class QSplitter;
class QAction;
class QIcon;
class QToolBar;
class QMenuBar;
class QMenu;

class OriginPacketText;
class ProtoTree;
class FilterLineEdit;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public: 
    MainWindow();
    ~MainWindow();

public slots:
    void tableviewSelect(const QItemSelection & selected);
    void prototreeSelectChanged(const QItemSelection& selected);
    void setCaptureOptions(const bool bPromiscuous, 
            const int deviceIndex, const QString& strFilter);

private:
    void createMainWidget();
    void createAction();
    void createMenu();
    void createToolBar();

private slots:
    void startCapture();
    void stopCapture();
    void searchFilter();

    // The Action slot
    void selectInterface();
    // Zoom
    void zoomIn();
    void zoomOut();
    void zoomOriginal();


private:
    MyTableView *tableview;
    ProtoTree *prototree;    
    QSplitter *mainspliter;
    OriginPacketText *packettext;
    FilterLineEdit* filteredit;

    // The Dialog
    InterfacesSelectDialog* interfacedialog;
    

    // Actions
    QAction* interfacesAction;          // List the aviliable capture interfaces
    QAction* captureOptionAction;       // Show the capture interfaces detail
    QAction* captureStartAction;        // Start a new live capture
    QAction* captureStopAction;         // Stop the running live caption
    QAction* openFileAction;            // Open a capture file
    QAction* saveFileAction;            // Save the capture file
    QAction* goPreviousPacketAction;    // Go back in packet history
    QAction* goNextPacketAction;        // Go forward in packet history
    QAction* goJumpPacketAction;        // Go to the packet With number
    QAction* goFirstPacketAction;       // Go to the first packet
    QAction* goLastPacketAction;        // Go to the last packet
    QAction* zoomInAction;              // Zoom in
    QAction* zoomOutAction;             // Zoom out
    QAction* zoomOriginalAction;        // Zoom 100%

    // Toolbar
    QToolBar* toolbar;                  // The toolbar 
    QMenuBar* menubar;                  // The menubar
    QMenu* fileMenu;                    // The file menu
    QMenu* viewMenu;                    // The view menu
    QMenu* captureMenu;                 // The capture menu
    QMenu* goMenu;                      // The go menu

    SnifferThread *pSnifferthread;
    Sniffer *pSniffer;


    // Zoom level
    int zoomLevel;                      // The zoom level max is 10, every zoom mean multipy 1.1 
};

#endif
