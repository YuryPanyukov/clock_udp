/********************************************************************************
** Form generated from reading UI file 'serverwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.12.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SERVERWINDOW_H
#define UI_SERVERWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTimeEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ServerWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QFormLayout *formLayout;
    QLabel *timeLabel;
    QTimeEdit *timeEdit;
    QLabel *seenLabel;
    QHBoxLayout *buttonsLayout;
    QPushButton *btnYes;
    QPushButton *btnNo;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *verticalSpacer;

    void setupUi(QMainWindow *ServerWindow)
    {
        if (ServerWindow->objectName().isEmpty())
            ServerWindow->setObjectName("ServerWindow");
        ServerWindow->resize(320, 180);
        centralwidget = new QWidget(ServerWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName("verticalLayout");
        formLayout = new QFormLayout();
        formLayout->setObjectName("formLayout");
        timeLabel = new QLabel(centralwidget);
        timeLabel->setObjectName("timeLabel");

        formLayout->setWidget(0, QFormLayout::ItemRole::LabelRole, timeLabel);

        timeEdit = new QTimeEdit(centralwidget);
        timeEdit->setObjectName("timeEdit");

        formLayout->setWidget(0, QFormLayout::ItemRole::FieldRole, timeEdit);


        verticalLayout->addLayout(formLayout);

        seenLabel = new QLabel(centralwidget);
        seenLabel->setObjectName("seenLabel");

        verticalLayout->addWidget(seenLabel);

        buttonsLayout = new QHBoxLayout();
        buttonsLayout->setObjectName("buttonsLayout");
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

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        buttonsLayout->addItem(horizontalSpacer);


        verticalLayout->addLayout(buttonsLayout);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        ServerWindow->setCentralWidget(centralwidget);

        retranslateUi(ServerWindow);

        QMetaObject::connectSlotsByName(ServerWindow);
    } // setupUi

    void retranslateUi(QMainWindow *ServerWindow)
    {
        ServerWindow->setWindowTitle(QCoreApplication::translate("ServerWindow", "\320\241\320\265\321\200\320\262\320\265\321\200 \321\207\320\260\321\201\320\276\320\262", nullptr));
        timeLabel->setText(QCoreApplication::translate("ServerWindow", "\320\222\321\200\320\265\320\274\321\217:", nullptr));
        timeEdit->setDisplayFormat(QCoreApplication::translate("ServerWindow", "HH:mm", nullptr));
        seenLabel->setText(QCoreApplication::translate("ServerWindow", "\320\232\320\273\320\270\320\265\320\275\321\202 \320\262\320\270\320\264\320\270\321\202 \320\262\321\200\320\265\320\274\321\217:", nullptr));
        btnYes->setText(QCoreApplication::translate("ServerWindow", "\320\224\320\260", nullptr));
        btnNo->setText(QCoreApplication::translate("ServerWindow", "\320\235\320\265\321\202", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ServerWindow: public Ui_ServerWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SERVERWINDOW_H
