/*
 * =====================================================================================
 *
 *       Filename:  originpacket_text.h
 *
 *    Description:  The text that display the origin packet bit and char
 *
 *        Version:  1.0
 *        Created:  2014年10月09日 22时08分37秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ping Zeng (zp), zengping@ncic.ac.cn
 *   Organization:  HPC Research Center
 *
 * =====================================================================================
 */

#ifndef UI_ORIGINPACKET_TEXT_H
#define UI_ORIGINPACKET_TEXT_H

#include <QtCore/QByteArray>
#include <QTextEdit>
#include <QMenu>
#include <QAction>
#include <QContextMenuEvent>

#include <memory.h>
#include "../include/snifferutil.h"

class QMenu;
class QAction;

class OriginPacketText : public QTextEdit
{
    Q_OBJECT

    public:
        OriginPacketText(QWidget* parent=0);
    
        void addData(const QByteArray&);

        /* *
         * Set the selection text
         * */
        void setSelection(int offset ,int len);

    private slots:
        void hexView();
        void bitsView();

    private:
        /* *
         * Set raw byte text and its char text
         * */
        void setDataText();


        void createContextMenu();

        /* *
         * The offset byte position
         * */
        int positionOfByteOffset(int offset, bool bEnd);

        /* *
         * The offset char position
         * */
        int positionOfCharOffset(int offset, bool bEnd);

        // Is hex view or bits view
        // The hex view will show 16 bytes while bits view show 8 bytes
        bool bHex;
        QString biggerSpace;
        QString litterSpace;
        QByteArray rawdata;

        QAction* hexAction;
        QAction* bitsAction;

};

#endif
