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
#include <QtWidgets/QRadioButton>
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
    QGroupBox *m_out_groupBox;
    QGroupBox *groupBox_2;
    QPushButton *m_reset_btn;
    QPushButton *m_stop_btn;
    QPushButton *m_play_btn;
    QRadioButton *m_radioBtn_Realtime;
    QRadioButton *m_radioBtn_offline;
    QPushButton *m_file_btn;
    QPushButton *m_dde_init_btn;
    QPushButton *m_dde_first_track_btn;
    QPushButton *m_dde_track_brn;
    QPushButton *m_calibration_btn;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *GaztCorrectionClass)
    {
        if (GaztCorrectionClass->objectName().isEmpty())
            GaztCorrectionClass->setObjectName(QStringLiteral("GaztCorrectionClass"));
        GaztCorrectionClass->resize(1327, 904);
        centralWidget = new QWidget(GaztCorrectionClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setGeometry(QRect(90, 540, 451, 311));
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
        m_groupBox->setGeometry(QRect(0, 0, 661, 501));
        m_out_groupBox = new QGroupBox(centralWidget);
        m_out_groupBox->setObjectName(QStringLiteral("m_out_groupBox"));
        m_out_groupBox->setGeometry(QRect(660, 0, 661, 501));
        groupBox_2 = new QGroupBox(centralWidget);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(590, 540, 661, 80));
        m_reset_btn = new QPushButton(groupBox_2);
        m_reset_btn->setObjectName(QStringLiteral("m_reset_btn"));
        m_reset_btn->setGeometry(QRect(290, 40, 75, 23));
        m_stop_btn = new QPushButton(groupBox_2);
        m_stop_btn->setObjectName(QStringLiteral("m_stop_btn"));
        m_stop_btn->setGeometry(QRect(210, 40, 75, 23));
        m_play_btn = new QPushButton(groupBox_2);
        m_play_btn->setObjectName(QStringLiteral("m_play_btn"));
        m_play_btn->setGeometry(QRect(130, 40, 75, 23));
        m_radioBtn_Realtime = new QRadioButton(groupBox_2);
        m_radioBtn_Realtime->setObjectName(QStringLiteral("m_radioBtn_Realtime"));
        m_radioBtn_Realtime->setGeometry(QRect(10, 20, 89, 16));
        m_radioBtn_offline = new QRadioButton(groupBox_2);
        m_radioBtn_offline->setObjectName(QStringLiteral("m_radioBtn_offline"));
        m_radioBtn_offline->setGeometry(QRect(10, 40, 89, 16));
        m_file_btn = new QPushButton(groupBox_2);
        m_file_btn->setObjectName(QStringLiteral("m_file_btn"));
        m_file_btn->setGeometry(QRect(370, 40, 75, 23));
        m_dde_init_btn = new QPushButton(groupBox_2);
        m_dde_init_btn->setObjectName(QStringLiteral("m_dde_init_btn"));
        m_dde_init_btn->setGeometry(QRect(130, 10, 75, 23));
        m_dde_first_track_btn = new QPushButton(groupBox_2);
        m_dde_first_track_btn->setObjectName(QStringLiteral("m_dde_first_track_btn"));
        m_dde_first_track_btn->setGeometry(QRect(210, 10, 101, 23));
        m_dde_track_brn = new QPushButton(groupBox_2);
        m_dde_track_brn->setObjectName(QStringLiteral("m_dde_track_brn"));
        m_dde_track_brn->setGeometry(QRect(320, 10, 75, 23));
        m_calibration_btn = new QPushButton(groupBox_2);
        m_calibration_btn->setObjectName(QStringLiteral("m_calibration_btn"));
        m_calibration_btn->setGeometry(QRect(530, 10, 121, 61));
        GaztCorrectionClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(GaztCorrectionClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1327, 23));
        GaztCorrectionClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(GaztCorrectionClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        GaztCorrectionClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(GaztCorrectionClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        GaztCorrectionClass->setStatusBar(statusBar);

        retranslateUi(GaztCorrectionClass);
        QObject::connect(m_play_btn, SIGNAL(clicked()), GaztCorrectionClass, SLOT(playVideo()));
        QObject::connect(m_stop_btn, SIGNAL(clicked()), GaztCorrectionClass, SLOT(stopVideo()));
        QObject::connect(m_reset_btn, SIGNAL(clicked()), GaztCorrectionClass, SLOT(terminateVideo()));
        QObject::connect(m_file_btn, SIGNAL(clicked()), GaztCorrectionClass, SLOT(file()));
        QObject::connect(m_radioBtn_Realtime, SIGNAL(clicked()), GaztCorrectionClass, SLOT(processMode()));
        QObject::connect(m_radioBtn_offline, SIGNAL(clicked()), GaztCorrectionClass, SLOT(processMode()));
        QObject::connect(m_dde_init_btn, SIGNAL(clicked()), GaztCorrectionClass, SLOT(dde_init()));
        QObject::connect(m_dde_first_track_btn, SIGNAL(clicked()), GaztCorrectionClass, SLOT(dde_first_track()));
        QObject::connect(m_dde_track_brn, SIGNAL(clicked()), GaztCorrectionClass, SLOT(dde_track()));
        QObject::connect(m_calibration_btn, SIGNAL(clicked()), GaztCorrectionClass, SLOT(calibration()));

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
        m_groupBox->setTitle(QApplication::translate("GaztCorrectionClass", "video input", 0));
        m_out_groupBox->setTitle(QApplication::translate("GaztCorrectionClass", "video output", 0));
        groupBox_2->setTitle(QApplication::translate("GaztCorrectionClass", "Controler", 0));
        m_reset_btn->setText(QApplication::translate("GaztCorrectionClass", "Reset", 0));
        m_stop_btn->setText(QApplication::translate("GaztCorrectionClass", "Stop", 0));
        m_play_btn->setText(QApplication::translate("GaztCorrectionClass", "Play", 0));
        m_radioBtn_Realtime->setText(QApplication::translate("GaztCorrectionClass", "RealTime", 0));
        m_radioBtn_offline->setText(QApplication::translate("GaztCorrectionClass", "Off Line", 0));
        m_file_btn->setText(QApplication::translate("GaztCorrectionClass", "File", 0));
        m_dde_init_btn->setText(QApplication::translate("GaztCorrectionClass", "DDE init", 0));
        m_dde_first_track_btn->setText(QApplication::translate("GaztCorrectionClass", "DDE track", 0));
        m_dde_track_brn->setText(QApplication::translate("GaztCorrectionClass", "DDE track", 0));
        m_calibration_btn->setText(QApplication::translate("GaztCorrectionClass", "Calibration", 0));
    } // retranslateUi

};

namespace Ui {
    class GaztCorrectionClass: public Ui_GaztCorrectionClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GAZTCORRECTION_H
