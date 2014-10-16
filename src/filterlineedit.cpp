/*
 * =====================================================================================
 *
 *       Filename:  filterlineedit.cpp
 *
 *    Description:  For the more see the ../include/filterlineedit.h
 *
 *        Version:  1.0
 *        Created:  2014年10月15日 17时52分37秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ping Zeng (zp), zengping@ncic.ac.cn
 *   Organization:  HPC Research Center
 *
 * =====================================================================================
 */
#include "../include/filterlineedit.h"

FilterLineEdit::FilterLineEdit(QWidget* parent) : QFrame(parent)
{
    this->model = new QStandardItemModel(0,1, this);
    this->completer = new QCompleter(this->model, this);

    this->lb = new QLabel(this);
    this->lb->setText("Filter:");

    this->setStyleSheet(QString("QLineEdit {"
                "border: 1px solid white;"
                "border-radius: 3px;"
                "font: mono;"
                "selection-background-color: blue;"
                "background-color: white;"
                "}"
                "QLineEdit::focus {"
                "border: 1px solid blue;"
                "}"));

    this->linetext = new QLineEdit(this);
    this->linetext->setMinimumHeight(30);
    this->linetext->setCompleter(this->completer);
    this->linetext->setPlaceholderText("" 
            "The filter column is (src/dest/protocal/info),the value can be the regex, such as protocal=ud.*");
    addCompleteStr();
    
    this->layout = new QHBoxLayout(this);
    this->layout->addWidget(this->lb);
    this->layout->addWidget(this->linetext);
    this->layout->addStretch();

    this->setLayout(layout);

    connect(completer, SIGNAL(activated(const QString&)), this, SLOT(onSearchChoosed(const QString&)));
    //connect(this->linetext, SIGNAL(textChanged(const QString&)), this, SLOT(onTextChanged(const QString&)));
    connect(this->linetext, SIGNAL(returnPressed()), this, SLOT(lineEditReturnPressed()));
}

void FilterLineEdit::onSearchChoosed(const QString& text)
{
    this->linetext->clear();
    this->linetext->setText(text);
}

void FilterLineEdit::onTextChanged(const QString& )
{
    QStringList strlist;
    strlist << "src = " << "dest = " << "protocol = " << "info = ";
 
    this->model->removeRows(0, this->model->rowCount());  
    for (int i = 0; i < strlist.size(); ++i)
    {
        this->model->insertRow(0);
        this->model->setData(this->model->index(0, 0), strlist.at(i));
    }
}

QString FilterLineEdit::getFilterText()
{
    return this->linetext->text();
}

void FilterLineEdit::lineEditReturnPressed()
{
    emit returnPressed();
}

void FilterLineEdit::addCompleteStr()
{
    QStringList strlist;
    strlist << "time > " << "time < " << "src = " << "dest = " << "protocol = " << "info = ";
 
    this->model->removeRows(0, this->model->rowCount());  
    for (int i = 0; i < strlist.size(); ++i)
    {
        this->model->insertRow(0);
        this->model->setData(this->model->index(0, 0), strlist.at(i));
    }
}
