/*
 * =====================================================================================
 *
 *       Filename:  treeitem.h
 *
 *    Description:  The item model constains in the prototree
 *
 *        Version:  1.0
 *        Created:  2014年10月15日 19时39分46秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ping Zeng (zp), zengping@ncic.ac.cn
 *   Organization:  HPC Research Center
 *
 * =====================================================================================
 */

#ifndef TREEITEM_H
#define TREEITEM_H

#include <QtCore/QVariant>

class TreeItem
{
    public:
        TreeItem();
        TreeItem(int offset, int len);

        void setOffset(int offset);
        void setLen(int len);

        int getOffset();
        int getLen();

    private:
        int offset;   // The offset of the node
        int len;      // The len of the node
};

typedef TreeItem *PTreeItem;
Q_DECLARE_METATYPE(TreeItem);

enum MyRoles {
    ProtoTreeRole = Qt::UserRole + 1 
};
Q_DECLARE_METATYPE(MyRoles);
#endif
