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
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GaztCorrectionClass
{
public:
    QWidget *centralWidget;
    QTabWidget *tabWidget;
    QWidget *in;
    QLabel *m_Image;
    QWidget *tab_3;
    QLabel *m_target_out_image;
    QWidget *out;
    QLabel *m_output_image;
    QWidget *tab;
    QLabel *m_transfer_iamge;
    QWidget *tab_2;
    QLabel *m_seamless_image;
    QGroupBox *m_groupBox;
    QPushButton *m_reset_btn;
    QPushButton *pushButton;
    QPushButton *m_stop_btn;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *GaztCorrectionClass)
    {
        if (GaztCorrectionClass->objectName().isEmpty())
            GaztCorrectionClass->setObjectName(QStringLiteral("GaztCorrectionClass"));
        GaztCorrectionClass->resize(932, 904);
        centralWidget = new QWidget(GaztCorrectionClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setGeometry(QRect(0, 390, 661, 461));
        in = new QWidget();
        in->setObjectName(QStringLiteral("in"));
        m_Image = new QLabel(in);
        m_Image->setObjectName(QStringLiteral("m_Image"));
        m_Image->setGeometry(QRect(0, 0, 461, 361));
        tabWidget->addTab(in, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QStringLiteral("tab_3"));
        m_target_out_image = new QLabel(tab_3);
        m_target_out_image->setObjectName(QStringLiteral("m_target_out_image"));
        m_target_out_image->setGeometry(QRect(0, 0, 461, 361));
        tabWidget->addTab(tab_3, QString());
        out = new QWidget();
        out->setObjectName(QStringLiteral("out"));
        m_output_image = new QLabel(out);
        m_output_image->setObjectName(QStringLiteral("m_output_image"));
        m_output_image->setGeometry(QRect(0, 0, 461, 361));
        tabWidget->addTab(out, QString());
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        m_transfer_iamge = new QLabel(tab);
        m_transfer_iamge->setObjectName(QStringLiteral("m_transfer_iamge"));
        m_transfer_iamge->setGeometry(QRect(0, 0, 461, 361));
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        m_seamless_image = new QLabel(tab_2);
        m_seamless_image->setObjectName(QStringLiteral("m_seamless_image"));
        m_seamless_image->setGeometry(QRect(0, 0, 461, 361));
        tabWidget->addTab(tab_2, QString());
        m_groupBox = new QGroupBox(centralWidget);
        m_groupBox->setObjectName(QStringLiteral("m_groupBox"));
        m_groupBox->setGeometry(QRect(0, 0, 641, 341));
        m_reset_btn = new QPushButton(m_groupBox);
        m_reset_btn->setObjectName(QStringLiteral("m_reset_btn"));
        m_reset_btn->setGeometry(QRect(180, 310, 75, 23));
        pushButton = new QPushButton(m_groupBox);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(20, 310, 75, 23));
        m_stop_btn = new QPushButton(m_groupBox);
        m_stop_btn->setObjectName(QStringLiteral("m_stop_btn"));
        m_stop_btn->setGeometry(QRect(100, 310, 75, 23));
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
        QObject::connect(pushButton, SIGNAL(clicked()), GaztCorrectionClass, SLOT(playVideo()));
        QObject::connect(m_stop_btn, SIGNAL(clicked()), GaztCorrectionClass, SLOT(stopVideo()));
        QObject::connect(m_reset_btn, SIGNAL(clicked()), GaztCorrectionClass, SLOT(terminateVideo()));

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(GaztCorrectionClass);
    } // setupUi

    void retranslateUi(QMainWindow *GaztCorrectionClass)
    {
        GaztCorrectionClass->setWindowTitle(QApplication::translate("GaztCorrectionClass", "GaztCorrection", 0));
        m_Image->setText(QApplication::translate("GaztCorrectionClass", "TextLabel", 0));
        tabWidget->setTabText(tabWidget->indexOf(in), QApplication::translate("GaztCorrectionClass", "in", 0));
        m_target_out_image->setText(QApplication::translate("GaztCorrectionClass", "TextLabel", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("GaztCorrectionClass", "target_out", 0));
        m_output_image->setText(QApplication::translate("GaztCorrectionClass", "TextLabel", 0));
        tabWidget->setTabText(tabWidget->indexOf(out), QApplication::translate("GaztCorrectionClass", "out", 0));
        m_transfer_iamge->setText(QApplication::translate("GaztCorrectionClass", "TextLabel", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("GaztCorrectionClass", "transfer", 0));
        m_seamless_image->setText(QApplication::translate("GaztCorrectionClass", "TextLabel", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("GaztCorrectionClass", "seamless", 0));
        m_groupBox->setTitle(QApplication::translate("GaztCorrectionClass", "vedio", 0));
        m_reset_btn->setText(QApplication::translate("GaztCorrectionClass", "Reset", 0));
        pushButton->setText(QApplication::translate("GaztCorrectionClass", "Play", 0));
        m_stop_btn->setText(QApplication::translate("GaztCorrectionClass", "Stop", 0));
    } // retranslateUi

};

namespace Ui {
    class GaztCorrectionClass: public Ui_GaztCorrectionClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GAZTCORRECTION_H
