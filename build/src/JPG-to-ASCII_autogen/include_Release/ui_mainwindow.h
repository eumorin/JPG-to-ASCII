/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.2.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_mainwindow
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QLineEdit *lineEdit;
    QPushButton *Choose;
    QPushButton *Convert;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *mainwindow)
    {
        if (mainwindow->objectName().isEmpty())
            mainwindow->setObjectName(QString::fromUtf8("mainwindow"));
        mainwindow->resize(357, 192);
        centralWidget = new QWidget(mainwindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label = new QLabel(centralWidget);
        label->setObjectName(QString::fromUtf8("label"));

        verticalLayout->addWidget(label);

        lineEdit = new QLineEdit(centralWidget);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        verticalLayout->addWidget(lineEdit);

        Choose = new QPushButton(centralWidget);
        Choose->setObjectName(QString::fromUtf8("Choose"));

        verticalLayout->addWidget(Choose);

        Convert = new QPushButton(centralWidget);
        Convert->setObjectName(QString::fromUtf8("Convert"));

        verticalLayout->addWidget(Convert);

        mainwindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(mainwindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 357, 22));
        mainwindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(mainwindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        mainwindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(mainwindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        mainwindow->setStatusBar(statusBar);

        retranslateUi(mainwindow);

        QMetaObject::connectSlotsByName(mainwindow);
    } // setupUi

    void retranslateUi(QMainWindow *mainwindow)
    {
        mainwindow->setWindowTitle(QCoreApplication::translate("mainwindow", "JPG-to-ASCII", nullptr));
        label->setText(QCoreApplication::translate("mainwindow", "\320\222\321\213\320\261\320\265\321\200\320\270\321\202\320\265, \321\201\320\272\320\276\320\273\321\214\320\272\320\276 \320\277\320\270\320\272\321\201\320\265\320\273\320\265\320\271 \321\204\320\276\321\202\320\276\320\263\321\200\320\260\321\204\320\270\320\270 \320\261\321\203\320\264\320\265\321\202 \320\277\321\200\320\265\320\264\321\201\321\202\320\260\320\262\320\273\321\217\321\202\321\214\n"
"\320\276\320\264\320\270\320\275 ASCII \321\201\320\270\320\274\320\262\320\276\320\273 (1-30)", nullptr));
        Choose->setText(QCoreApplication::translate("mainwindow", "Choose an image", nullptr));
        Convert->setText(QCoreApplication::translate("mainwindow", "Convert and save", nullptr));
    } // retranslateUi

};

namespace Ui {
    class mainwindow: public Ui_mainwindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
