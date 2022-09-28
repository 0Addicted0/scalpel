#ifndef WIDGET_H
#define WIDGET_H
#include <QMetaType>
#include <QWidget>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QFile>
#include <QProcess>
#include <QTime>
#include <QStack>
#include <QTimer>
#include <QThread>
#include <QBrush>
#include <QColor>
#include <QTreeWidgetItem>
#include <windows.h>
#include <psapi.h>
#include "sharedMem.h"
#include "behaviorJudge.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE


class emunModule :public QThread
{
    Q_OBJECT
public:
    emunModule();
    ~emunModule();
    DWORD dwProcessId;
    QString moduleStr;
signals:
    void updateModule(QString);
protected:
    void run() override;
};

class scalpelInjectThread:public QThread
{
    Q_OBJECT

    public:
        scalpelInjectThread();
        ~scalpelInjectThread();
        QString receptorPath,receptorName,receptorPID,receptorModule,receptorPriority;
        HANDLE receptorHandle;
        HANDLE hSemaphore, hMapFile, hLockerSemaphore;
        bool processState;
        bool killInjector();
        scalpel::sharedM::hookedInfo recvInfo;
        emunModule getModule;
        bool filter(uint8_t op);
        bool fileFilter, heapFilter, regFilter, netFilter;
    signals:
        void injectedProcessStartSig();
        void injectedProcessStopSig();
        void updateInfo(QString);
        void messageBoxInfo(QString, QString);
        //void updateModule();
        void updateStatisticsInfo(char,bool);
        void messageHint(QString, QString);
        void messageWarn(QString, QString);
    protected:
        void run() override;
    private:
        QString priorityStr[8] = { "NORMAL", "IDLE" , "REALTIME", "HIGH", "NULL", "ABOVENORMAL", "BELOWNORMAL" };
        int GetProcessPriority(HANDLE hProcess)
        {
            switch (GetPriorityClass(hProcess))
            {
                case NORMAL_PRIORITY_CLASS:return 0;
                case  IDLE_PRIORITY_CLASS:return 1;
                case REALTIME_PRIORITY_CLASS:return 2;
                case HIGH_PRIORITY_CLASS:return 3;
                case ABOVE_NORMAL_PRIORITY_CLASS:return 5;
                case BELOW_NORMAL_PRIORITY_CLASS:return 6;
                default:return 4;
            }
        }
        void visualization();
        bool inspect(QString &ret, int &isWarn);
        LPVOID lpBase;//ptr to  shared mem
};
class Widget : public QWidget
{
    Q_OBJECT

    public:
        Widget(QWidget *parent = nullptr);
        ~Widget();
        void updateLineWithStop(QString suffix);
        bool pathInspect(QString filePath,QString &fileName);
        void initScreen();
        QString operNum2Str(int num);
//signals:
    //void updateFilter(QPushButton*, bool&);
signals:
    void updateModuleStop(QString);
    private slots:
        void updatePIDLine(QString pid);
        void updateTimeLine(QString time);
        void updateNameLine(QString name);
        void updateModuleLine(QString mod);
        void on_chooseHeap_clicked();
        void on_chooseFile_clicked();
        void on_chooseReg_clicked();
        void on_chooseNet_clicked();
        void on_fileButton_clicked();
        void on_commitButton_clicked();
        void on_cancelButton_clicked();
        void set_startIcon();
        void set_stopIcon();
        void startTimertoShow();
        void stopTimertoShow();
        void injectedThreadStart();
        void injectedThreadStop();
        void injectedThreadUpateInfo(QString st);
        void injectedThreadUpateStatisticsInfo(char title,bool safe);
        void injectedThreadHint(QString hint, QString st);
        void injectedThreadWarn(QString warn, QString st);
        void wakeUpmessageBox(QString _class, QString _cont)
        {
            QMessageBox::warning(this, _class, _cont);
            return;
        }
    private:
        Ui::Widget *ui;
        qint8 hDiffer,mDiffer,sDiffer;
        qint16 msDiffer;
        bool hasPushedStop,hasPushedStart;
        QTimer* timerForShow;
        scalpelInjectThread injector;
        
};
#endif // WIDGET_H
