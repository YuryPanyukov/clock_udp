/********************************************************************************
** Form generated from reading UI file 'clientwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.12.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CLIENTWINDOW_H
#define UI_CLIENTWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "clockface.h"

QT_BEGIN_NAMESPACE

class Ui_ClientWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    ClockFace *clockWidget;
    QLabel *actualLabel;
    QHBoxLayout *buttonsLayout;
    QSpacerItem *leftSpacer;
    QPushButton *btnYes;
    QPushButton *btnNo;
    QSpacerItem *rightSpacer;

    void setupUi(QMainWindow *ClientWindow)
    {
        if (ClientWindow->objectName().isEmpty())
            ClientWindow->setObjectName("ClientWindow");
        ClientWindow->resize(400, 500);
        centralwidget = new QWidget(ClientWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName("verticalLayout");
        clockWidget = new ClockFace(centralwidget);
        clockWidget->setObjectName("clockWidget");
        clockWidget->setMinimumSize(QSize(300, 300));

        verticalLayout->addWidget(clockWidget);

        actualLabel = new QLabel(centralwidget);
        actualLabel->setObjectName("actualLabel");
        actualLabel->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(actualLabel);

        buttonsLayout = new QHBoxLayout();
        buttonsLayout->setObjectName("buttonsLayout");
        leftSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        buttonsLayout->addItem(leftSpacer);

        btnYes = new QPushButton(centralwidget);
        btnYes->setObjectName("btnYes");
        btnYes->setMinimumSize(QSize(60, 0));
        btnYes->setFocusPolicy(Qt::NoFocus);
        btnYes->setCheckable(true);

        buttonsLayout->addWidget(btnYes);

        btnNo = new QPushButton(centralwidget);
        btnNo->setObjectName("btnNo");
        btnNo->setMinimumSize(QSize(60, 0));
        btnNo->setFocusPolicy(Qt::NoFocus);
        btnNo->setCheckable(true);

        buttonsLayout->addWidget(btnNo);

        rightSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        buttonsLayout->addItem(rightSpacer);


        verticalLayout->addLayout(buttonsLayout);

        ClientWindow->setCentralWidget(centralwidget);

        retranslateUi(ClientWindow);

        QMetaObject::connectSlotsByName(ClientWindow);
    } // setupUi

    void retranslateUi(QMainWindow *ClientWindow)
    {
        ClientWindow->setWindowTitle(QCoreApplication::translate("ClientWindow", "\320\232\320\273\320\270\320\265\320\275\321\202 \321\207\320\260\321\201\320\276\320\262", nullptr));
        actualLabel->setText(QCoreApplication::translate("ClientWindow", "\320\224\320\260\320\275\320\275\321\213\320\265 \320\276 \320\262\321\200\320\265\320\274\320\265\320\275\320\270 \320\260\320\272\321\202\321\203\320\260\320\273\321\214\320\275\321\213:", nullptr));
        btnYes->setText(QCoreApplication::translate("ClientWindow", "\320\224\320\260", nullptr));
        btnNo->setText(QCoreApplication::translate("ClientWindow", "\320\235\320\265\321\202", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ClientWindow: public Ui_ClientWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CLIENTWINDOW_H
