/********************************************************************************
** Form generated from reading UI file 'gaztcorrection.ui'
**
** Created by: Qt User Interface Compiler version 5.3.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GAZTCORRECTION_H
#define UI_GAZTCORRECTION_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GaztCorrectionClass
{
public:
    QWidget *centralWidget;
    QLabel *m_Image;
    QLabel *m_output_image;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *GaztCorrectionClass)
    {
        if (GaztCorrectionClass->objectName().isEmpty())
            GaztCorrectionClass->setObjectName(QStringLiteral("GaztCorrectionClass"));
        GaztCorrectionClass->resize(932, 643);
        centralWidget = new QWidget(GaztCorrectionClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        m_Image = new QLabel(centralWidget);
        m_Image->setObjectName(QStringLiteral("m_Image"));
        m_Image->setGeometry(QRect(10, 0, 461, 361));
        m_output_image = new QLabel(centralWidget);
        m_output_image->setObjectName(QStringLiteral("m_output_image"));
        m_output_image->setGeometry(QRect(420, 0, 511, 361));
        GaztCorrectionClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(GaztCorrectionClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 932, 23));
        GaztCorrectionClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(GaztCorrectionClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        GaztCorrectionClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(GaztCorrectionClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        GaztCorrectionClass->setStatusBar(statusBar);

        retranslateUi(GaztCorrectionClass);

        QMetaObject::connectSlotsByName(GaztCorrectionClass);
    } // setupUi

    void retranslateUi(QMainWindow *GaztCorrectionClass)
    {
        GaztCorrectionClass->setWindowTitle(QApplication::translate("GaztCorrectionClass", "GaztCorrection", 0));
        m_Image->setText(QApplication::translate("GaztCorrectionClass", "TextLabel", 0));
        m_output_image->setText(QApplication::translate("GaztCorrectionClass", "TextLabel", 0));
    } // retranslateUi

};

namespace Ui {
    class GaztCorrectionClass: public Ui_GaztCorrectionClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GAZTCORRECTION_H
