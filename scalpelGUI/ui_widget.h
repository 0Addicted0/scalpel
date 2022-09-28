/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QFrame *eyebrowLine;
    QLabel *toolLabel;
    QPushButton *fileButton;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout;
    QPushButton *commitButton;
    QPushButton *cancelButton;
    QLineEdit *pathLine;
    QWidget *layoutWidget1;
    QVBoxLayout *verticalLayout;
    QLabel *processTip;
    QTreeWidget *processScreen;
    QWidget *layoutWidget2;
    QVBoxLayout *verticalLayout_2;
    QLabel *totalTip;
    QTreeWidget *totalScreen;
    QWidget *layoutWidget3;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout_5;
    QSpacerItem *verticalSpacer_2;
    QVBoxLayout *verticalLayout_4;
    QLabel *exeNameTip;
    QLabel *timerTip;
    QLabel *PIDtip;
    QLabel *priorityTip;
    QLabel *Moudletip;
    QSpacerItem *verticalSpacer;
    QVBoxLayout *verticalLayout_3;
    QVBoxLayout *verticalLayout_7;
    QVBoxLayout *verticalLayout_8;
    QLineEdit *exeNameLine;
    QLineEdit *timerLine;
    QLineEdit *PIDLine;
    QLineEdit *priorityLine;
    QSpacerItem *verticalSpacer_3;
    QTreeWidget *ModuleLine;
    QWidget *layoutWidget_2;
    QVBoxLayout *verticalLayout_6;
    QLabel *messageTip;
    QTreeWidget *message;
    QWidget *layoutWidget4;
    QGridLayout *gridLayout;
    QPushButton *chooseHeap;
    QPushButton *chooseFile;
    QPushButton *chooseNet;
    QPushButton *chooseReg;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QString::fromUtf8("Widget"));
        Widget->resize(1800, 800);
        Widget->setMaximumSize(QSize(1800, 800));
        Widget->setWindowOpacity(0.960000000000000);
        eyebrowLine = new QFrame(Widget);
        eyebrowLine->setObjectName(QString::fromUtf8("eyebrowLine"));
        eyebrowLine->setGeometry(QRect(20, 10, 1391, 20));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        eyebrowLine->setFont(font);
        eyebrowLine->setFrameShape(QFrame::HLine);
        eyebrowLine->setFrameShadow(QFrame::Sunken);
        toolLabel = new QLabel(Widget);
        toolLabel->setObjectName(QString::fromUtf8("toolLabel"));
        toolLabel->setGeometry(QRect(1390, 490, 81, 31));
        QFont font1;
        font1.setFamily(QString::fromUtf8("\351\273\221\344\275\223"));
        font1.setPointSize(12);
        toolLabel->setFont(font1);
        fileButton = new QPushButton(Widget);
        fileButton->setObjectName(QString::fromUtf8("fileButton"));
        fileButton->setGeometry(QRect(1730, 680, 35, 35));
        fileButton->setAutoFillBackground(true);
        fileButton->setFlat(true);
        layoutWidget = new QWidget(Widget);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(1420, 730, 291, 41));
        horizontalLayout = new QHBoxLayout(layoutWidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        commitButton = new QPushButton(layoutWidget);
        commitButton->setObjectName(QString::fromUtf8("commitButton"));
        commitButton->setMinimumSize(QSize(0, 35));
        QFont font2;
        font2.setFamily(QString::fromUtf8("\351\273\221\344\275\223"));
        font2.setPointSize(10);
        commitButton->setFont(font2);
        commitButton->setFlat(true);

        horizontalLayout->addWidget(commitButton);

        cancelButton = new QPushButton(layoutWidget);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));
        cancelButton->setMinimumSize(QSize(0, 35));
        cancelButton->setFont(font2);
        cancelButton->setFlat(true);

        horizontalLayout->addWidget(cancelButton);

        pathLine = new QLineEdit(Widget);
        pathLine->setObjectName(QString::fromUtf8("pathLine"));
        pathLine->setGeometry(QRect(1420, 680, 291, 35));
        pathLine->setMinimumSize(QSize(0, 35));
        pathLine->setFont(font2);
        pathLine->setAcceptDrops(true);
        pathLine->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        pathLine->setReadOnly(false);
        pathLine->setClearButtonEnabled(true);
        layoutWidget1 = new QWidget(Widget);
        layoutWidget1->setObjectName(QString::fromUtf8("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(20, 30, 911, 751));
        verticalLayout = new QVBoxLayout(layoutWidget1);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        processTip = new QLabel(layoutWidget1);
        processTip->setObjectName(QString::fromUtf8("processTip"));
        processTip->setFont(font1);

        verticalLayout->addWidget(processTip);

        processScreen = new QTreeWidget(layoutWidget1);
        QFont font3;
        font3.setFamily(QString::fromUtf8("\351\273\221\344\275\223"));
        font3.setPointSize(11);
        processScreen->headerItem()->setText(0, QString());
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setFont(0, font3);
        processScreen->setHeaderItem(__qtreewidgetitem);
        processScreen->setObjectName(QString::fromUtf8("processScreen"));
        processScreen->setFont(font3);
        processScreen->setFocusPolicy(Qt::StrongFocus);
        processScreen->setToolTipDuration(-1);
        processScreen->setAutoFillBackground(false);
        processScreen->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        processScreen->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        processScreen->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
        processScreen->setDragDropOverwriteMode(true);
        processScreen->setWordWrap(false);
        processScreen->setHeaderHidden(false);
        processScreen->header()->setHighlightSections(true);

        verticalLayout->addWidget(processScreen);

        layoutWidget2 = new QWidget(Widget);
        layoutWidget2->setObjectName(QString::fromUtf8("layoutWidget2"));
        layoutWidget2->setGeometry(QRect(950, 30, 421, 331));
        verticalLayout_2 = new QVBoxLayout(layoutWidget2);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        totalTip = new QLabel(layoutWidget2);
        totalTip->setObjectName(QString::fromUtf8("totalTip"));
        totalTip->setFont(font1);

        verticalLayout_2->addWidget(totalTip);

        totalScreen = new QTreeWidget(layoutWidget2);
        totalScreen->headerItem()->setText(0, QString());
        QTreeWidgetItem *__qtreewidgetitem1 = new QTreeWidgetItem();
        __qtreewidgetitem1->setFont(0, font3);
        totalScreen->setHeaderItem(__qtreewidgetitem1);
        totalScreen->setObjectName(QString::fromUtf8("totalScreen"));
        totalScreen->setFont(font3);
        totalScreen->setLineWidth(1);
        totalScreen->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        totalScreen->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        totalScreen->setDragDropOverwriteMode(true);
        totalScreen->setHeaderHidden(false);

        verticalLayout_2->addWidget(totalScreen);

        layoutWidget3 = new QWidget(Widget);
        layoutWidget3->setObjectName(QString::fromUtf8("layoutWidget3"));
        layoutWidget3->setGeometry(QRect(1390, 30, 371, 451));
        horizontalLayout_2 = new QHBoxLayout(layoutWidget3);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setSpacing(7);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_5->addItem(verticalSpacer_2);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setSpacing(28);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(0, 0, 0, 0);
        exeNameTip = new QLabel(layoutWidget3);
        exeNameTip->setObjectName(QString::fromUtf8("exeNameTip"));
        exeNameTip->setMinimumSize(QSize(0, 35));
        exeNameTip->setFont(font1);

        verticalLayout_4->addWidget(exeNameTip);

        timerTip = new QLabel(layoutWidget3);
        timerTip->setObjectName(QString::fromUtf8("timerTip"));
        timerTip->setMinimumSize(QSize(0, 35));
        timerTip->setFont(font1);

        verticalLayout_4->addWidget(timerTip);

        PIDtip = new QLabel(layoutWidget3);
        PIDtip->setObjectName(QString::fromUtf8("PIDtip"));
        PIDtip->setMinimumSize(QSize(0, 35));
        PIDtip->setFont(font1);

        verticalLayout_4->addWidget(PIDtip);

        priorityTip = new QLabel(layoutWidget3);
        priorityTip->setObjectName(QString::fromUtf8("priorityTip"));
        priorityTip->setMinimumSize(QSize(0, 35));
        priorityTip->setFont(font1);

        verticalLayout_4->addWidget(priorityTip);

        Moudletip = new QLabel(layoutWidget3);
        Moudletip->setObjectName(QString::fromUtf8("Moudletip"));
        Moudletip->setMinimumSize(QSize(0, 35));
        Moudletip->setFont(font1);

        verticalLayout_4->addWidget(Moudletip);

        verticalLayout_4->setStretch(1, 6);
        verticalLayout_4->setStretch(2, 6);
        verticalLayout_4->setStretch(3, 6);
        verticalLayout_4->setStretch(4, 6);

        verticalLayout_5->addLayout(verticalLayout_4);

        verticalSpacer = new QSpacerItem(20, 1, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_5->addItem(verticalSpacer);

        verticalLayout_5->setStretch(0, 2);
        verticalLayout_5->setStretch(1, 30);
        verticalLayout_5->setStretch(2, 25);

        horizontalLayout_2->addLayout(verticalLayout_5);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(5);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setSizeConstraint(QLayout::SetMaximumSize);
        verticalLayout_3->setContentsMargins(0, 0, 0, 1);
        verticalLayout_7 = new QVBoxLayout();
        verticalLayout_7->setSpacing(15);
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        verticalLayout_7->setContentsMargins(-1, 0, -1, -1);
        verticalLayout_8 = new QVBoxLayout();
        verticalLayout_8->setSpacing(28);
        verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
        verticalLayout_8->setContentsMargins(-1, 20, -1, -1);
        exeNameLine = new QLineEdit(layoutWidget3);
        exeNameLine->setObjectName(QString::fromUtf8("exeNameLine"));
        exeNameLine->setMinimumSize(QSize(0, 35));
        exeNameLine->setFont(font2);
        exeNameLine->setAlignment(Qt::AlignCenter);
        exeNameLine->setReadOnly(true);

        verticalLayout_8->addWidget(exeNameLine);

        timerLine = new QLineEdit(layoutWidget3);
        timerLine->setObjectName(QString::fromUtf8("timerLine"));
        timerLine->setMinimumSize(QSize(0, 35));
        timerLine->setFont(font2);
        timerLine->setAlignment(Qt::AlignCenter);
        timerLine->setDragEnabled(false);
        timerLine->setReadOnly(true);
        timerLine->setClearButtonEnabled(false);

        verticalLayout_8->addWidget(timerLine);

        PIDLine = new QLineEdit(layoutWidget3);
        PIDLine->setObjectName(QString::fromUtf8("PIDLine"));
        PIDLine->setMinimumSize(QSize(0, 35));
        PIDLine->setFont(font2);
        PIDLine->setAlignment(Qt::AlignCenter);
        PIDLine->setReadOnly(true);

        verticalLayout_8->addWidget(PIDLine);

        priorityLine = new QLineEdit(layoutWidget3);
        priorityLine->setObjectName(QString::fromUtf8("priorityLine"));
        priorityLine->setMinimumSize(QSize(0, 35));
        priorityLine->setFont(font2);
        priorityLine->setAlignment(Qt::AlignCenter);
        priorityLine->setReadOnly(true);

        verticalLayout_8->addWidget(priorityLine);


        verticalLayout_7->addLayout(verticalLayout_8);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_7->addItem(verticalSpacer_3);

        ModuleLine = new QTreeWidget(layoutWidget3);
        ModuleLine->headerItem()->setText(0, QString());
        QTreeWidgetItem *__qtreewidgetitem2 = new QTreeWidgetItem();
        __qtreewidgetitem2->setFont(0, font3);
        ModuleLine->setHeaderItem(__qtreewidgetitem2);
        ModuleLine->setObjectName(QString::fromUtf8("ModuleLine"));
        ModuleLine->setFont(font3);
        ModuleLine->setLineWidth(1);
        ModuleLine->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        ModuleLine->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        ModuleLine->setDragDropOverwriteMode(true);
        ModuleLine->setHeaderHidden(true);
        ModuleLine->header()->setVisible(false);

        verticalLayout_7->addWidget(ModuleLine);

        verticalLayout_7->setStretch(0, 8);
        verticalLayout_7->setStretch(1, 1);
        verticalLayout_7->setStretch(2, 14);

        verticalLayout_3->addLayout(verticalLayout_7);


        horizontalLayout_2->addLayout(verticalLayout_3);

        layoutWidget_2 = new QWidget(Widget);
        layoutWidget_2->setObjectName(QString::fromUtf8("layoutWidget_2"));
        layoutWidget_2->setGeometry(QRect(950, 370, 421, 411));
        verticalLayout_6 = new QVBoxLayout(layoutWidget_2);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        verticalLayout_6->setContentsMargins(0, 0, 0, 0);
        messageTip = new QLabel(layoutWidget_2);
        messageTip->setObjectName(QString::fromUtf8("messageTip"));
        messageTip->setFont(font1);

        verticalLayout_6->addWidget(messageTip);

        message = new QTreeWidget(layoutWidget_2);
        message->headerItem()->setText(0, QString());
        QTreeWidgetItem *__qtreewidgetitem3 = new QTreeWidgetItem();
        __qtreewidgetitem3->setFont(0, font3);
        message->setHeaderItem(__qtreewidgetitem3);
        message->setObjectName(QString::fromUtf8("message"));
        message->setFont(font3);
        message->setLineWidth(1);
        message->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        message->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        message->setDragDropOverwriteMode(true);
        message->setHeaderHidden(true);
        message->header()->setVisible(false);

        verticalLayout_6->addWidget(message);

        layoutWidget4 = new QWidget(Widget);
        layoutWidget4->setObjectName(QString::fromUtf8("layoutWidget4"));
        layoutWidget4->setGeometry(QRect(1410, 520, 311, 151));
        gridLayout = new QGridLayout(layoutWidget4);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setHorizontalSpacing(8);
        gridLayout->setVerticalSpacing(16);
        gridLayout->setContentsMargins(8, 8, 8, 8);
        chooseHeap = new QPushButton(layoutWidget4);
        chooseHeap->setObjectName(QString::fromUtf8("chooseHeap"));
        chooseHeap->setMinimumSize(QSize(0, 40));
        chooseHeap->setFont(font1);
        chooseHeap->setFlat(true);

        gridLayout->addWidget(chooseHeap, 0, 1, 1, 1);

        chooseFile = new QPushButton(layoutWidget4);
        chooseFile->setObjectName(QString::fromUtf8("chooseFile"));
        chooseFile->setMinimumSize(QSize(0, 40));
        chooseFile->setFont(font1);
        chooseFile->setFlat(true);

        gridLayout->addWidget(chooseFile, 0, 2, 1, 1);

        chooseNet = new QPushButton(layoutWidget4);
        chooseNet->setObjectName(QString::fromUtf8("chooseNet"));
        chooseNet->setMinimumSize(QSize(0, 40));
        chooseNet->setFont(font1);
        chooseNet->setFlat(true);

        gridLayout->addWidget(chooseNet, 1, 2, 1, 1);

        chooseReg = new QPushButton(layoutWidget4);
        chooseReg->setObjectName(QString::fromUtf8("chooseReg"));
        chooseReg->setMinimumSize(QSize(0, 40));
        chooseReg->setFont(font1);
        chooseReg->setFlat(true);

        gridLayout->addWidget(chooseReg, 1, 1, 1, 1);


        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QCoreApplication::translate("Widget", "Scalpel", nullptr));
        toolLabel->setText(QCoreApplication::translate("Widget", "\347\255\233\351\200\211\345\210\227\350\241\250", nullptr));
        fileButton->setText(QString());
        commitButton->setText(QCoreApplication::translate("Widget", "\345\274\200\345\247\213", nullptr));
        cancelButton->setText(QCoreApplication::translate("Widget", "\347\273\210\346\255\242", nullptr));
#if QT_CONFIG(whatsthis)
        pathLine->setWhatsThis(QCoreApplication::translate("Widget", "<html><head/><body><p><br/></p></body></html>", nullptr));
#endif // QT_CONFIG(whatsthis)
        pathLine->setText(QCoreApplication::translate("Widget", "\350\257\267\350\276\223\345\205\245\350\267\257\345\276\204", nullptr));
        processTip->setText(QCoreApplication::translate("Widget", "\345\256\236\346\227\266\347\233\221\346\265\213", nullptr));
#if QT_CONFIG(tooltip)
        processScreen->setToolTip(QString());
#endif // QT_CONFIG(tooltip)
        totalTip->setText(QCoreApplication::translate("Widget", "\347\273\237\350\256\241\347\273\223\346\236\234", nullptr));
        exeNameTip->setText(QCoreApplication::translate("Widget", "\347\250\213\345\272\217\345\220\215\347\247\260", nullptr));
        timerTip->setText(QCoreApplication::translate("Widget", "\346\211\247\350\241\214\346\227\266\351\227\264", nullptr));
        PIDtip->setText(QCoreApplication::translate("Widget", "\350\277\233\347\250\213\347\274\226\345\217\267", nullptr));
        priorityTip->setText(QCoreApplication::translate("Widget", "\350\277\233\347\250\213\344\274\230\345\205\210\347\272\247", nullptr));
        Moudletip->setText(QCoreApplication::translate("Widget", "\350\275\275\345\205\245\346\250\241\345\235\227", nullptr));
        exeNameLine->setText(QCoreApplication::translate("Widget", "NULL", nullptr));
        timerLine->setText(QCoreApplication::translate("Widget", "00:00:00:000", nullptr));
#if QT_CONFIG(whatsthis)
        PIDLine->setWhatsThis(QCoreApplication::translate("Widget", "<html><head/><body><p><br/></p></body></html>", nullptr));
#endif // QT_CONFIG(whatsthis)
        PIDLine->setText(QCoreApplication::translate("Widget", "NULL", nullptr));
#if QT_CONFIG(whatsthis)
        priorityLine->setWhatsThis(QCoreApplication::translate("Widget", "<html><head/><body><p><br/></p></body></html>", nullptr));
#endif // QT_CONFIG(whatsthis)
        priorityLine->setText(QCoreApplication::translate("Widget", "NULL", nullptr));
        messageTip->setText(QCoreApplication::translate("Widget", "\346\266\210\346\201\257", nullptr));
        chooseHeap->setText(QCoreApplication::translate("Widget", "\345\240\206\346\223\215\344\275\234   ", nullptr));
        chooseFile->setText(QCoreApplication::translate("Widget", "\346\226\207\344\273\266\346\223\215\344\275\234", nullptr));
        chooseNet->setText(QCoreApplication::translate("Widget", "\347\275\221\347\273\234\346\223\215\344\275\234", nullptr));
        chooseReg->setText(QCoreApplication::translate("Widget", "\346\263\250\345\206\214\350\241\250\346\223\215\344\275\234", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
