/*
 * =====================================================================================
 *
 *       Filename:  treeitem.cpp
 *
 *    Description:  For the more see ../include/treeitem.h
 *
 *        Version:  1.0
 *        Created:  2014年10月15日 19时42分52秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ping Zeng (zp), zengping@ncic.ac.cn
 *   Organization:  HPC Research Center
 *
 * =====================================================================================
 */

#include "../include/treeitem.h"

TreeItem::TreeItem()
{
    this->offset = 0;
    this->len = 0;
}

TreeItem::TreeItem(int offset, int len)
{
    this->offset = offset;
    this->len = len;
}

int TreeItem::getOffset()
{
    return this->offset;
}

int TreeItem::getLen()
{
    return this->len;
}

void TreeItem::setOffset(int offset)
{
    this->offset = offset;
}

void TreeItem::setLen(int len)
{
    this->len = len;
}
