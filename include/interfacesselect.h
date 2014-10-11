/*
 * =====================================================================================
 *
 *       Filename:  interfacesselect.h
 *
 *    Description:  The interfaces selected form
 *
 *        Version:  1.0
 *        Created:  2014年10月12日 00时56分09秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ping Zeng (zp), zengping@ncic.ac.cn
 *   Organization:  HPC Research Center
 *
 * =====================================================================================
 */
#ifndef INTERFACESSELECT_H_
#define INTERFACESSELECT_H_

#include "../ui/ui_interfacesselect.h"

class InterfacesSelect : public QFrame, public Ui::InterfacesSelect
{
    public:
        InterfacesSelect(QWidget* parent=0);
        ~InterfacesSelect();
};

#endif
