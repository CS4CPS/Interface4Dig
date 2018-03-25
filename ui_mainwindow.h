/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QPushButton *pb_net;
    QLineEdit *le_netpar;
    QFrame *line;
    QGroupBox *gb_path;
    QRadioButton *rb_default;
    QRadioButton *rb_;
    QLineEdit *le_path;
    QPushButton *pushButton;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(500, 400);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        pb_net = new QPushButton(centralWidget);
        pb_net->setObjectName(QStringLiteral("pb_net"));
        pb_net->setGeometry(QRect(280, 10, 100, 22));
        le_netpar = new QLineEdit(centralWidget);
        le_netpar->setObjectName(QStringLiteral("le_netpar"));
        le_netpar->setGeometry(QRect(100, 10, 160, 22));
        line = new QFrame(centralWidget);
        line->setObjectName(QStringLiteral("line"));
        line->setGeometry(QRect(10, 40, 480, 16));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        gb_path = new QGroupBox(centralWidget);
        gb_path->setObjectName(QStringLiteral("gb_path"));
        gb_path->setGeometry(QRect(30, 60, 451, 80));
        rb_default = new QRadioButton(gb_path);
        rb_default->setObjectName(QStringLiteral("rb_default"));
        rb_default->setGeometry(QRect(40, 20, 121, 17));
        rb_ = new QRadioButton(gb_path);
        rb_->setObjectName(QStringLiteral("rb_"));
        rb_->setGeometry(QRect(180, 20, 131, 17));
        le_path = new QLineEdit(gb_path);
        le_path->setObjectName(QStringLiteral("le_path"));
        le_path->setGeometry(QRect(20, 50, 411, 20));
        pushButton = new QPushButton(gb_path);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(340, 20, 91, 23));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 500, 17));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        pb_net->setText(QApplication::translate("MainWindow", "PushButton", 0));
        gb_path->setTitle(QApplication::translate("MainWindow", "Profile Path", 0));
        rb_default->setText(QApplication::translate("MainWindow", "Default File", 0));
        rb_->setText(QApplication::translate("MainWindow", "Specify File", 0));
        pushButton->setText(QApplication::translate("MainWindow", "Open", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
