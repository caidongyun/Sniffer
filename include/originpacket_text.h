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
#include "../include/snifferutil.h"

class OriginPacketText : public QTextEdit
{
    Q_OBJECT

    public:
        OriginPacketText(QWidget* parent=0);
    
        void addData(const QByteArray&);

    private:
        QString biggerSpace;
        QString litterSpace;

};

#endif
