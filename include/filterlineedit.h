/*
 * =====================================================================================
 *
 *       Filename:  filterlineedit.h
 *
 *    Description:  The lineedit in the search area including autocomplete
 *
 *        Version:  1.0
 *        Created:  2014年10月15日 02时41分15秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ping Zeng (zp), zengping@ncic.ac.cn
 *   Organization:  HPC Research Center
 *
 * =====================================================================================
 */
#ifndef FILTERLINEEDIT_H
#define FILTERLINEEDIT_H

#include <QLineEdit>
#include <QLabel>
#include <QStandardItemModel>
#include <QCompleter>
#include <QHBoxLayout>

class QLineEdit;
class QLabel;
class QHBoxLayout;
class QStandardItemModel;
class QCompleter;

class FilterLineEdit : public QFrame
{
    Q_OBJECT

    public:
        FilterLineEdit(QWidget* parent=0);
        QString getFilterText();

    signals:
        void returnPressed();

    private slots:
        void onSearchChoosed(const QString& text);
        void onTextChanged(const QString& str);
        void lineEditReturnPressed();
        void addCompleteStr();

    private:
        QStandardItemModel* model;
        QCompleter* completer;
        QLineEdit* linetext;
        QLabel* lb; // The prompt text
        QHBoxLayout* layout;
};

#endif
