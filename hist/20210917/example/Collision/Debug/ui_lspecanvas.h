/********************************************************************************
** Form generated from reading UI file 'lspecanvas.ui'
**
** Created by: Qt User Interface Compiler version 5.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LSPECANVAS_H
#define UI_LSPECANVAS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LspeCanvas
{
public:

    void setupUi(QWidget *LspeCanvas)
    {
        if (LspeCanvas->objectName().isEmpty())
            LspeCanvas->setObjectName(QStringLiteral("LspeCanvas"));
        LspeCanvas->resize(600, 600);
        LspeCanvas->setAutoFillBackground(false);
        LspeCanvas->setStyleSheet(QStringLiteral(""));

        retranslateUi(LspeCanvas);

        QMetaObject::connectSlotsByName(LspeCanvas);
    } // setupUi

    void retranslateUi(QWidget *LspeCanvas)
    {
        LspeCanvas->setWindowTitle(QApplication::translate("LspeCanvas", "Form", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class LspeCanvas: public Ui_LspeCanvas {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LSPECANVAS_H
