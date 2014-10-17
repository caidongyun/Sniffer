/*
 * =====================================================================================
 *
 *       Filename:  mainwindow.cpp
 *
 *    Description:  主窗口实现文件
 *
 *        Version:  1.0
 *        Created:  2014年09月21日 20时14分29秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ping Zeng (zp), zengping@ncic.ac.cn
 *   Organization:  HPC Research Center
 *
 * =====================================================================================
 */
#include "../include/mainwindow.h"

MainWindow::MainWindow()
{
    
    this->pSniffer = new Sniffer();
    if (pSniffer->getAllNetDevs() == false)
    {
        QMessageBox::warning(this, tr("Sniffer"),
                tr("<H3>无法在您的机器上获取可用的网络适配器接口</H3>"
                    "<P>可能有如下原因:</P>"
                    "<ul><li>不支持您的网卡,请到 <a href=\"http://winpcap.org\">"
                    "http://winpcap.org</a> 查阅支持的硬件列表\n</li>"
                    "<li>您的杀毒软件或者防火墙阻止本软件运行</li></ul>"), 
                    QMessageBox::Ok);
    }

    createMainWidget();
    createAction();
    createMenu();
    createToolBar();
    
}

MainWindow::~MainWindow()
{
    if (this->pSniffer)
    {
        this->pSniffer->deleteLater();
        this->pSniffer = NULL;
    }
    if (this->pSnifferthread)
    {
        this->pSnifferthread->stopSniffer();
        this->pSnifferthread->deleteLater();
        this->pSnifferthread = NULL;
    }
}

void MainWindow::createMainWidget()
{
    this->setWindowTitle("Sniffer");
    this->setWindowState(Qt::WindowMaximized);
    this->setGeometry(0,0,960,768);

    this->tableview = new MyTableView(this);
    this->prototree = new ProtoTree(this);
    this->packettext = new OriginPacketText(this);
    this->filteredit = new FilterLineEdit(this);
    this->interfacedialog = new InterfacesSelectDialog(pSniffer, this);

    this->mainspliter = new QSplitter(Qt::Vertical,this);

    this->mainspliter->addWidget(this->filteredit);
    this->mainspliter->addWidget(this->tableview);
    this->mainspliter->addWidget(this->prototree);
    this->mainspliter->addWidget(this->packettext);
    
    this->setCentralWidget(this->mainspliter);

    // Binding tableview selection action
    QObject::connect(this->tableview->selectionModel(), 
            SIGNAL(selectionChanged( const QItemSelection&, const QItemSelection&)),
            this, SLOT(tableviewSelect(const QItemSelection&)));


    // Bind the prototree selection
    QObject::connect(this->prototree->selectionModel(),
            SIGNAL(selectionChanged( const QItemSelection&, const QItemSelection&)),
            this, SLOT(prototreeSelectChanged(const QItemSelection&)));
    
    // Binding searchlinetext returnPressed
    QObject::connect(this->filteredit,SIGNAL(returnPressed()),
            this, SLOT(searchFilter()));

    // Bind the interface select options
    QObject::connect(this->interfacedialog, SIGNAL(setCaptureOptions(const bool,
                    const int, const QString&)), this,
                SLOT(setCaptureOptions(const bool, const int, const QString&)));
}

void MainWindow::createAction()
{
    this->interfacesAction = new QAction(tr("Interfaces"), this);
    this->interfacesAction->setIcon(QIcon(":/images/capture_interfaces.png"));
    this->interfacesAction->setShortcut(tr("Ctrl+B"));
    this->interfacesAction->setToolTip(tr("List the aviliable capture interfaces"));

    this->captureOptionAction = new QAction(tr("Options"), this);
    this->captureOptionAction->setIcon(QIcon(":/images/x-capture-options.png"));
    this->captureOptionAction->setShortcut(tr("Ctrl+K"));
    this->captureOptionAction->setToolTip(tr("Show the capture interfaces detail"));
    this->captureOptionAction->setEnabled(true);
    QObject::connect(this->captureOptionAction, SIGNAL(triggered()),
            this,SLOT(selectInterface()));
    
    this->captureStartAction = new QAction(tr("Start"), this);
    this->captureStartAction->setIcon(QIcon(":/images/x-capture-start.png"));
    this->captureStartAction->setShortcut(tr("Ctrl+E"));
    this->captureStartAction->setToolTip(tr("Start a new live capture"));
    this->captureStartAction->setEnabled(false);
    QObject::connect(captureStartAction, SIGNAL(triggered()), this, SLOT(startCapture()));
    
    this->captureStopAction= new QAction(tr("Stop"), this);
    this->captureStopAction->setIcon(QIcon(":/images/x-capture-stop.png"));
    this->captureStopAction->setShortcut(tr("Ctrl+E"));
    this->captureStopAction->setToolTip(tr("Stop the running live caption"));
    this->captureStopAction->setEnabled(false);
    QObject::connect(captureStopAction, SIGNAL(triggered()), this, SLOT(stopCapture()));
    
    this->openFileAction = new QAction(tr("Open"), this);
    this->openFileAction->setIcon(this->style()->standardIcon(QStyle::SP_DirIcon));
    this->openFileAction->setShortcut(QKeySequence::Open);
    this->openFileAction->setToolTip(tr("Open a capture file"));
    
    this->saveFileAction = new QAction(tr("Save"), this);
    this->saveFileAction->setIcon(QIcon(":/images/x-capture-file-save.png"));
    this->saveFileAction->setShortcut(QKeySequence::Save);
    this->saveFileAction->setToolTip(tr("Save the capture file"));
    
    this->goPreviousPacketAction = new QAction(tr("Previous Packet"), this);
    this->goPreviousPacketAction->setIcon(QIcon(":/images/go-previous.png"));
    this->goPreviousPacketAction->setShortcut(QKeySequence::Back);
    this->goPreviousPacketAction->setToolTip(tr("Go back in packet history"));
    this->goPreviousPacketAction->setEnabled(false);
    
    this->goNextPacketAction = new QAction(tr("Next Packet"), this);
    this->goNextPacketAction->setIcon(QIcon(":/images/go-next.png"));
    this->goNextPacketAction->setShortcut(QKeySequence::Forward);
    this->goNextPacketAction->setToolTip(tr("Go forward in packet history"));
    this->goNextPacketAction->setEnabled(false);

    this->goJumpPacketAction = new QAction(tr("Go to Packet"), this);
    this->goJumpPacketAction->setIcon(QIcon(":/images/go-jump.png"));
    this->goJumpPacketAction->setShortcut(tr("Ctrl+G"));
    this->goJumpPacketAction->setToolTip(tr("Go to the packet With number"));
    this->goJumpPacketAction->setEnabled(false);
    
    this->goFirstPacketAction = new QAction(tr("First Packet"), this);
    this->goFirstPacketAction->setIcon(QIcon(":/images/go-first.png"));
    this->goFirstPacketAction->setShortcut(tr("Ctrl+Home"));
    this->goFirstPacketAction->setToolTip(tr("Go to the first packet"));
    this->goFirstPacketAction->setEnabled(false);
     
    this->goLastPacketAction = new QAction(tr("Last Packet"), this);
    this->goLastPacketAction->setIcon(QIcon(":/images/go-last.png"));
    this->goLastPacketAction->setShortcut(tr("Ctrl+End"));
    this->goLastPacketAction->setToolTip(tr("Go to the last packet"));
    this->goLastPacketAction->setEnabled(false);

    this->zoomInAction = new QAction(tr("Zoom In"), this);
    this->zoomInAction->setIcon(QIcon(":/images/zoom-in.png"));
    this->zoomInAction->setShortcut(QKeySequence::ZoomIn);
    this->zoomInAction->setToolTip(tr("Zoom In"));
    this->zoomInAction->setEnabled(false);
    QObject::connect(this->zoomInAction, SIGNAL(triggered()),
            this, SLOT(zoomIn()));

    this->zoomOutAction = new QAction(tr("Zoom Out"), this);
    this->zoomOutAction->setIcon(QIcon(":/images/zoom-out.png"));
    this->zoomOutAction->setShortcut(QKeySequence::ZoomOut);
    this->zoomOutAction->setToolTip(tr("Zoom Out"));
    this->zoomOutAction->setEnabled(false);
    QObject::connect(this->zoomOutAction, SIGNAL(triggered()),
            this, SLOT(zoomOut()));

    this->zoomOriginalAction = new QAction(tr("Normal Size"), this);
    this->zoomOriginalAction->setIcon(QIcon(":/images/zoom-original.png"));
    this->zoomOriginalAction->setShortcut(tr("Ctrl+="));
    this->zoomOriginalAction->setToolTip(tr("Zoom 100%"));
    this->zoomOriginalAction->setEnabled(false);
    QObject::connect(this->zoomOriginalAction, SIGNAL(triggered()),
            this, SLOT(zoomOriginal()));

}

void MainWindow::createMenu()
{
    this->menubar = new QMenuBar(this);

    this->fileMenu = new QMenu("File");
    this->fileMenu->addAction(this->openFileAction);
    this->fileMenu->addAction(this->saveFileAction);
    this->menubar->addMenu(this->fileMenu);

    this->viewMenu = new QMenu("View");
    this->viewMenu->addAction(this->zoomInAction);
    this->viewMenu->addAction(this->zoomOutAction);
    this->viewMenu->addAction(this->zoomOriginalAction);
    this->menubar->addMenu(this->viewMenu);

    this->goMenu = new QMenu("Go");
    this->goMenu->addAction(this->goPreviousPacketAction);
    this->goMenu->addAction(this->goNextPacketAction);
    this->goMenu->addAction(this->goJumpPacketAction);
    this->goMenu->addAction(this->goFirstPacketAction);
    this->goMenu->addAction(this->goLastPacketAction);
    this->menubar->addMenu(this->goMenu);

    this->captureMenu = new QMenu("Capture");
    //this->captureMenu->addAction(this->interfacesAction);
    this->captureMenu->addAction(this->captureOptionAction);
    this->captureMenu->addAction(this->captureStartAction);
    this->captureMenu->addAction(this->captureStopAction);
    this->menubar->addMenu(this->captureMenu);

    this->setMenuBar(this->menubar);
}

void MainWindow::createToolBar()
{
   this->toolbar = new QToolBar(this); 
   //this->toolbar->addAction(this->interfacesAction);
   this->toolbar->addAction(this->captureOptionAction);
   this->toolbar->addAction(this->captureStartAction);
   this->toolbar->addAction(this->captureStopAction);
   this->toolbar->addSeparator();

   this->toolbar->addAction(this->openFileAction);
   this->toolbar->addAction(this->saveFileAction);
   this->toolbar->addSeparator();

   this->toolbar->addAction(this->goPreviousPacketAction);
   this->toolbar->addAction(this->goNextPacketAction);
   this->toolbar->addAction(this->goJumpPacketAction);
   this->toolbar->addAction(this->goFirstPacketAction);
   this->toolbar->addAction(this->goLastPacketAction);
   this->toolbar->addSeparator();

   this->toolbar->addAction(this->zoomInAction);
   this->toolbar->addAction(this->zoomOutAction);
   this->toolbar->addAction(this->zoomOriginalAction);

   this->addToolBar(Qt::TopToolBarArea, this->toolbar);
   
}

void MainWindow::startCapture()
{
    this->captureStartAction->setIcon(QIcon(":/images/x-capture-start.png"));
    this->captureStartAction->setEnabled(false); 
    this->captureStopAction->setEnabled(true); 

    this->pSnifferthread = new SnifferThread(pSniffer, this->tableview);
    connect(this->pSnifferthread,SIGNAL(addPacketItem(QString, QString, QString,QString, int, QString)),
            this->tableview,SLOT(addPacketItem(QString, QString, QString,QString, int, QString)));

    // Clear the data
    this->pSniffer->snifferDataVector.clear();
    this->tableview->clearData();

    this->pSnifferthread->start();
}

void MainWindow::stopCapture()
{
    this->captureStartAction->setIcon(QIcon(":/images/x-capture-start.on.png"));
    this->captureStartAction->setEnabled(true);
    this->captureStopAction->setEnabled(false);

    if (this->pSnifferthread)
    {
        this->pSnifferthread->stopSniffer();
        this->pSnifferthread->deleteLater();
        this->pSnifferthread = NULL;
    }
}

void MainWindow::tableviewSelect( const QItemSelection & selected)
{
    int row = selected.indexes().at(0).row();

    this->packettext->addData(this->pSniffer->snifferDataVector.at(row).rawData); 
    this->prototree->startAnalysis(this->pSniffer->snifferDataVector.at(row), row+1);
}

void MainWindow::prototreeSelectChanged(const QItemSelection& selected)
{
    QModelIndex index = selected.indexes().at(0);
    
    this->packettext->setSelection(index.data(ROLE_OFFSET).value<int>(),
            index.data(ROLE_LEN).value<int>());
}

void MainWindow::searchFilter()
{
    this->tableview->setFilterString(this->filteredit->getFilterText());
}


void MainWindow::selectInterface()
{
    this->interfacedialog->show();
}

void MainWindow::setCaptureOptions(const bool bPromiscuous,
        const int deviceIndex, const QString& strFilter)
{
    this->pSniffer->openNetDev(deviceIndex, bPromiscuous);
    if (strFilter != "")
    {
        this->pSniffer->setDeviceFilter(strFilter);
    }

    startCapture();

    // Enable the actions
    this->captureStopAction->setEnabled(true);
    this->goJumpPacketAction->setEnabled(true);
    this->goLastPacketAction->setEnabled(true);
    this->goNextPacketAction->setEnabled(true);
    this->goFirstPacketAction->setEnabled(true);
    this->goPreviousPacketAction->setEnabled(true);
    
    this->zoomOutAction->setEnabled(true);
    this->zoomOriginalAction->setEnabled(true);
    this->zoomInAction->setEnabled(true);
}

void MainWindow::zoomIn()
{
    this->tableview->zoomTextSize(++zoomLevel);
}

void MainWindow::zoomOut()
{
    this->tableview->zoomTextSize(--zoomLevel);
}

void MainWindow::zoomOriginal()
{
    this->tableview->zoomTextSize(0);
    this->zoomLevel = 0;
}
