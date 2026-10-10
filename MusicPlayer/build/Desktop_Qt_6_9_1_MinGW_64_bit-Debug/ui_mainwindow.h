/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_progressBar
{
public:
    QWidget *centralwidget;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout;
    QPushButton *prevBtn;
    QPushButton *playBtn;
    QPushButton *nextBtn;
    QPushButton *modeBtn;
    QPushButton *listBtn;
    QListWidget *musicList;
    QLabel *coverLabel;
    QPushButton *openDirBtn;
    QSlider *horizontalSlider;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *progressBar)
    {
        if (progressBar->objectName().isEmpty())
            progressBar->setObjectName("progressBar");
        progressBar->resize(1280, 720);
        centralwidget = new QWidget(progressBar);
        centralwidget->setObjectName("centralwidget");
        layoutWidget = new QWidget(centralwidget);
        layoutWidget->setObjectName("layoutWidget");
        layoutWidget->setGeometry(QRect(20, 450, 831, 321));
        horizontalLayout = new QHBoxLayout(layoutWidget);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        prevBtn = new QPushButton(layoutWidget);
        prevBtn->setObjectName("prevBtn");

        horizontalLayout->addWidget(prevBtn);

        playBtn = new QPushButton(layoutWidget);
        playBtn->setObjectName("playBtn");

        horizontalLayout->addWidget(playBtn);

        nextBtn = new QPushButton(layoutWidget);
        nextBtn->setObjectName("nextBtn");

        horizontalLayout->addWidget(nextBtn);

        modeBtn = new QPushButton(layoutWidget);
        modeBtn->setObjectName("modeBtn");

        horizontalLayout->addWidget(modeBtn);

        listBtn = new QPushButton(layoutWidget);
        listBtn->setObjectName("listBtn");

        horizontalLayout->addWidget(listBtn);

        musicList = new QListWidget(centralwidget);
        musicList->setObjectName("musicList");
        musicList->setGeometry(QRect(870, 10, 401, 571));
        QFont font;
        font.setFamilies({QString::fromUtf8("Haettenschweiler")});
        font.setPointSize(18);
        font.setWeight(QFont::Light);
        font.setStyleStrategy(QFont::PreferAntialias);
        musicList->setFont(font);
        musicList->setAutoFillBackground(true);
        musicList->setStyleSheet(QString::fromUtf8("background-color: rgba(255, 255, 255, 60);"));
        musicList->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAsNeeded);
        musicList->setAutoScroll(true);
        coverLabel = new QLabel(centralwidget);
        coverLabel->setObjectName("coverLabel");
        coverLabel->setGeometry(QRect(130, 70, 300, 300));
        coverLabel->setStyleSheet(QString::fromUtf8("background-color: rgba(255, 255, 255, 30);\n"
"border-radius: 10px;"));
        coverLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);
        openDirBtn = new QPushButton(centralwidget);
        openDirBtn->setObjectName("openDirBtn");
        openDirBtn->setGeometry(QRect(1010, 610, 80, 18));
        horizontalSlider = new QSlider(centralwidget);
        horizontalSlider->setObjectName("horizontalSlider");
        horizontalSlider->setGeometry(QRect(100, 400, 601, 16));
        horizontalSlider->setOrientation(Qt::Orientation::Horizontal);
        progressBar->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(progressBar);
        statusbar->setObjectName("statusbar");
        progressBar->setStatusBar(statusbar);

        retranslateUi(progressBar);

        QMetaObject::connectSlotsByName(progressBar);
    } // setupUi

    void retranslateUi(QMainWindow *progressBar)
    {
        progressBar->setWindowTitle(QCoreApplication::translate("progressBar", "MainWindow", nullptr));
        prevBtn->setText(QString());
        playBtn->setText(QString());
        nextBtn->setText(QString());
        modeBtn->setText(QString());
        listBtn->setText(QString());
        coverLabel->setText(QCoreApplication::translate("progressBar", "\345\260\201\351\235\242", nullptr));
        openDirBtn->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class progressBar: public Ui_progressBar {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
