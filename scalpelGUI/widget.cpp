#include "widget.h"
#include "ui_widget.h"
#include <iostream>
#include <cstdio>
#include <detours.h>
#ifdef WIN32  
#pragma execution_character_set("utf-8")  
#endif
#define TIMEOUT 1

#pragma comment(lib,"detours.lib")
#pragma comment(lib,"psapi.lib")

Widget::Widget(QWidget *parent)
    : QWidget(parent), ui(new Ui::Widget)
{
    ui->setupUi(this);
    initScreen();
    hasPushedStart = false; hasPushedStop = false;
    QIcon folderIcon(QCoreApplication::applicationDirPath()+"/image/folder.png");
    ui->fileButton->setIcon(folderIcon);
    ui->fileButton->setIconSize(QSize(35,35));
    set_startIcon();
    QIcon stopIcon(QCoreApplication::applicationDirPath()+"/image/stoped.png");
    ui->cancelButton->setIcon(stopIcon);
    ui->cancelButton->setIconSize(QSize(35,35));
    timerForShow = new QTimer(this);
    ui->message->setRootIsDecorated(false);
    ui->ModuleLine->setRootIsDecorated(false);

    QIcon noFilter(QCoreApplication::applicationDirPath()+"/image/button1.png");
    ui->chooseHeap->setIcon(noFilter);
    ui->chooseHeap->setIconSize(QSize(35, 35));
    ui->chooseNet->setIcon(noFilter);
    ui->chooseNet->setIconSize(QSize(35, 35));
    ui->chooseFile->setIcon(noFilter);
    ui->chooseFile->setIconSize(QSize(35, 35));
    ui->chooseReg->setIcon(noFilter);
    ui->chooseReg->setIconSize(QSize(35, 35));

    injector.heapFilter = false;
    injector.fileFilter = false;
    injector.netFilter = false;
    injector.regFilter = false;

    //connect(ui->chooseFile, SIGNAL(clicked()), this,  SLOT(changeFileFilter(fileFilter)));
    //connect(ui->chooseReg,  SIGNAL(clicked()), this,  SLOT(changeRegFilter(regFilter)));
    //connect(ui->chooseNet,  SIGNAL(clicked()), this,  SLOT(changeNetFilter(netFilter)));
    //connect(ui->chooseHeap, SIGNAL(clicked()), this,  SLOT(changeHeapFilter(heapFilter)));

    //ui->chooseHeap->setIconSize();
    //ui->processScreen->resizeColumnToContents(0);
    //hide ledftList
    //ui->message->setStyleSheet("QTreeView::branch:has-children:!has-siblings:closed,\
    //                            QTreeView::branch:closed:has-children:has-siblings{border-image: none; image: none;}\
    //                            QTreeView::branch:open:has-children:!has-siblings,\
    //                            QTreeView::branch:open:has-children:has-siblings{border-image: none; image: none;}");

    //ui->ModuleLine->setStyleSheet("QTreeView::branch:has-children:!has-siblings:closed,\
    //                            QTreeView::branch:closed:has-children:has-siblings{border-image: none; image: none;}\
    //                            QTreeView::branch:open:has-children:!has-siblings,\
    //                            QTreeView::branch:open:has-children:has-siblings{border-image: none; image: none;}");

    //LF
    connect(ui->pathLine,SIGNAL(returnPressed()),this,SLOT(on_commitButton_clicked()));
    //processStart
    connect(&injector, SIGNAL(injectedProcessStartSig()), this, SLOT(injectedThreadStart()));
    //processStop(manual)
    connect(&injector, SIGNAL(injectedProcessStopSig()), this, SLOT(injectedThreadStop()));
    //processStop(auto)
    connect(&injector, SIGNAL(updateInfo(QString)), this, SLOT(injectedThreadUpateInfo(QString)), Qt::BlockingQueuedConnection);
    //connect(&injector, SIGNAL(updateInfo(QString)), this, SLOT(injectedThreadUpateInfo(QString)));
    //processStart
    connect(&injector, SIGNAL(updateStatisticsInfo(char, bool)), this, SLOT(injectedThreadUpateStatisticsInfo(char, bool)), Qt::BlockingQueuedConnection);
    //connect(&injector, SIGNAL(updateStatisticsInfo(char, bool)), this, SLOT(injectedThreadUpateStatisticsInfo(char, bool)));
    //updateModule
    connect(&injector.getModule, SIGNAL(updateModule(QString)), this, SLOT(updateModuleLine(QString)));
    //message 
    connect(&injector, SIGNAL(messageHint(QString, QString)), this, SLOT(injectedThreadHint(QString, QString)), Qt::BlockingQueuedConnection);
    connect(&injector, SIGNAL(messageWarn(QString, QString)), this, SLOT(injectedThreadWarn(QString, QString)), Qt::BlockingQueuedConnection);
    //connect(&injector, SIGNAL(messageHint(QString, QString)), this, SLOT(injectedThreadHint(QString, QString)));
    //connect(&injector, SIGNAL(messageWarn(QString, QString)), this, SLOT(injectedThreadWarn(QString, QString)));
    //updateTimer
    connect(&injector, SIGNAL(messageBoxInfo(QString, QString)), this, SLOT(wakeUpmessageBox(QString, QString)));
    connect(timerForShow, &QTimer::timeout,[=]{
        msDiffer++;
        if(msDiffer==1000)
        {
            msDiffer=0;
            sDiffer++;
            if(sDiffer==60)
            {
                sDiffer=0;
                mDiffer++;
                if(mDiffer==60)
                {
                    mDiffer=0;
                    hDiffer++;
                }
            }
        }
        QString timeDiff;timeDiff.sprintf("%02d:%02d:%02d.%03d",hDiffer,mDiffer,sDiffer,msDiffer);
        updateTimeLine(timeDiff);});
}

Widget::~Widget()
{
    delete ui;
}
scalpelInjectThread::scalpelInjectThread()
{
    processState = false;
    return;
}
scalpelInjectThread::~scalpelInjectThread()
{
    return;
}
emunModule::emunModule()
{
    return;
}
emunModule::~emunModule()
{
    return;
}
void Widget::updatePIDLine(QString pid)
{
    ui->PIDLine->setText(pid);
}
void Widget::updateNameLine(QString name)
{
    ui->exeNameLine->setText(name);
}
void Widget::updateTimeLine(QString time)
{
    ui->timerLine->setText(time);
}
void Widget::updateModuleLine(QString mod)
{
    QTreeWidgetItem* item = new QTreeWidgetItem();
    item->setText(0,mod);
    ui->ModuleLine->addTopLevelItem(item);
    return;
}
void Widget::on_chooseHeap_clicked()
{
    QIcon noFilter(QCoreApplication::applicationDirPath()+"/image/button1.png");
    QIcon Filter(QCoreApplication::applicationDirPath()+"/image/button0.png");
    if (injector.heapFilter)
    {
        ui->chooseHeap->setIcon(noFilter);
        injector.heapFilter = false;
    }
    else
    {
        ui->chooseHeap->setIcon(Filter);
        injector.heapFilter = true;
    }
    return;
}
void Widget::on_chooseFile_clicked()
{
    QIcon noFilter(QCoreApplication::applicationDirPath()+"/image/button1.png");
    QIcon Filter(QCoreApplication::applicationDirPath()+"/image/button0.png");
    if (injector.fileFilter)
    {
        ui->chooseFile->setIcon(noFilter);
        injector.fileFilter = false;
    }
    else
    {
        ui->chooseFile->setIcon(Filter);
        injector.fileFilter = true;
    }
    return;
}
void Widget::on_chooseNet_clicked()
{
    QIcon noFilter(QCoreApplication::applicationDirPath()+"/image/button1.png");
    QIcon Filter(QCoreApplication::applicationDirPath()+"/image/button0.png");
    if (injector.netFilter)
    {
        ui->chooseNet->setIcon(noFilter);
        injector.netFilter = false;
    }
    else
    {
        ui->chooseNet->setIcon(Filter);
        injector.netFilter = true;
    }
    return;
}
void Widget::on_chooseReg_clicked()
{
    QIcon noFilter(QCoreApplication::applicationDirPath()+"/image/button1.png");
    QIcon Filter(QCoreApplication::applicationDirPath()+"/image/button0.png");
    if (injector.regFilter)
    {
        ui->chooseReg->setIcon(noFilter);
        injector.regFilter = false;
    }
    else
    {
        ui->chooseReg->setIcon(Filter);
        injector.regFilter = true;
    }
    //qDebug() << "in on_chooseReg: " << injector.regFilter << endl;
    return;
}
void Widget::on_fileButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,"选择一个可执行文件",QDir::homePath(),
                                 tr("Executable File (*.exe *.dll *.ocx)"));
    if(fileName.isEmpty())
    {
        if(ui->pathLine->text().isEmpty())
            QMessageBox::warning(this,"警告","请选择一个文件");
        //QMessageBox::warning(this, "警告", );

    }
    else
    {
        ui->pathLine->setText(fileName);
//        on_commitButton_clicked();
    }
}
bool Widget::pathInspect(QString filePath, QString &fileName)
{
    // 文件存在
    QFile file(filePath);
    if(!file.exists())
    {
        QMessageBox::warning(this,"警告","请选择一个文件");
        return false;
    }
    file.close();
    filePath.replace("/", "\\");
    QStringList list = filePath.split("\\");
    //QStringList list2 = list[list.length()-1].split('\\');
    fileName = list[list.length()-1];
    // 后缀合法
    //if((!fileName.contains(".exe",Qt::CaseInsensitive)) && (!fileName.contains(".ocx",Qt::CaseInsensitive)) && (!fileName.contains('.dll',Qt::CaseInsensitive)))
    //{
    //    QMessageBox::warning(this,"警告","文件类型不合法");
    //    return false;
    //}
    return true;
}
void Widget::on_commitButton_clicked()
{
    // 已在运行中
    if(injector.processState)
    {
        QMessageBox::warning(this,"警告","请先终止正在运行的程序");
        return;
    }
    //qDebug() << "here" << endl;
    QString filePath = ui->pathLine->text();
    QString fileName;
    if (!pathInspect(filePath, fileName))
    {
        return;
    }

    injector.receptorPath = filePath;
    injector.receptorName = fileName;
    //qDebug() << filePath<<" "<< fileName << endl;

    injector.start();
    return;
}
bool scalpelInjectThread::killInjector()
{
    return TerminateProcess(receptorHandle,-1);
}
void Widget::on_cancelButton_clicked()
{
    if(injector.processState)
    {
        if(!injector.killInjector())
        {
            QMessageBox::information(NULL,"警告","未能杀死程序");
            return;
        }
        injectedThreadStop();
        injector.processState = false;
    }
    else
    {
        QMessageBox::information(NULL,"提示","没有执行中的程序");
    }
    return;
}
void Widget::updateLineWithStop(QString suffix)
{
    QString old_text;
    old_text = ui->exeNameLine->text();
    ui->exeNameLine ->setText(old_text+suffix);
    old_text = ui->timerLine->text();
    ui->timerLine ->setText(old_text+suffix);
    old_text = ui->PIDLine->text();
    ui->PIDLine->setText(old_text+suffix);
    old_text = "(已终止)";
    updateModuleLine(old_text);
    old_text = ui->priorityLine->text();
    ui->priorityLine->setText(old_text+suffix);
    return;
}
void Widget::set_startIcon()
{
    QString suffix = "start1.png";
    if(hasPushedStart)suffix = "started1.png";
    QIcon startIcon(QCoreApplication::applicationDirPath()+"/image/"+suffix);
    ui->commitButton->setIcon(startIcon);
    ui->commitButton->setIconSize(QSize(35,35));
    return;
}
void Widget::set_stopIcon()
{
    QString suffix = "stop.png";
    if(hasPushedStop)suffix = "stoped.png";
    QIcon stopIcon(QCoreApplication::applicationDirPath()+"/image/"+suffix);
    ui->cancelButton->setIcon(stopIcon);
    ui->cancelButton->setIconSize(QSize(35,35));
    return;
}
void Widget::stopTimertoShow()
{
    if(timerForShow->isActive())
        timerForShow->stop();
    return;
}
void Widget::startTimertoShow()
{
    if(timerForShow->isActive() == false)
        timerForShow->start(TIMEOUT);
    return;
}
void Widget::injectedThreadStop()
{
    injector.processState = false;
    stopTimertoShow();

    updateLineWithStop("(已终止)");

    hasPushedStop=true;
    hasPushedStart=false;
    set_stopIcon();
    set_startIcon();

    CloseHandle(injector.hSemaphore);
    CloseHandle(injector.hMapFile);
    return;
}
void Widget::injectedThreadStart()
{
    initScreen();
    injector.processState = true;
    injector.receptorModule.clear();

    ui->timerLine ->setText("00:00:00.000");
    hDiffer = mDiffer = sDiffer = msDiffer = 0;
    startTimertoShow();

    hasPushedStart = true;
    hasPushedStop = false;
    set_startIcon();
    set_stopIcon();

    ui->priorityLine->setText(injector.receptorPriority);
    ui->exeNameLine->setText(injector.receptorName);
    ui->PIDLine->setText(injector.receptorPID);
    ui->ModuleLine->clear();
    injector.getModule.moduleStr.clear();
    return;
}
void scalpelInjectThread::run()
{
    //prepare for shared mem
    DWORD ms_time = 4;
    size_t structSize = sizeof (scalpel::sharedM::hookedInfo);
    if (!scalpel::sharedM::CreateSharedMem(structSize, hSemaphore, hMapFile, lpBase))
    {
        emit messageBoxInfo("提示", "创建内存映射失败");
        //qDebug()<<"创建内存映射失败" << endl;
        return;
    }
    if (!scalpel::sharedM::CreateLocker(hLockerSemaphore))
    {
        emit messageBoxInfo("提示", "创建信号锁失败");
        //qDebug() << "创建信号锁失败" << endl;
        return;
    }
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(STARTUPINFO));
    ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
    si.cb = sizeof(STARTUPINFO);
    
    QString qDir = QCoreApplication::applicationDirPath();
    QString q_path = qDir + "/scalpelDLL";
    QString q_dllpath  = q_path + "/scalpelDLL.dll";
    QByteArray dirPath = q_path.toUtf8();
    QByteArray dll =  q_dllpath.toUtf8();
    //strcpy_s(DirPath, "/scalpelDLL");
    char *DirPath = dirPath.data();
    char * DLLPath = dll.data();
    //strcpy_s(DLLPath, "/scalpelDLL/scalpelDLL.dll");
    //char* EXEPath = receptorPath.toLocal8Bit().data();
    char * EXEPath = receptorPath.toUtf8().data();
    QString inspectRET;
    scalpel::judge::initJudger();
    if (DetourCreateProcessWithDllExA(EXEPath, NULL, NULL, NULL, TRUE,
        CREATE_DEFAULT_ERROR_MODE | CREATE_SUSPENDED, NULL, DirPath, &si, &pi,
        DLLPath, NULL))
    {
        ResumeThread(pi.hThread);
        receptorPID = QString::number(pi.dwProcessId);
        receptorPriority = priorityStr[GetProcessPriority(pi.hProcess)];
        receptorHandle = pi.hProcess;
        emit injectedProcessStartSig();
        getModule.dwProcessId = pi.dwProcessId;
        getModule.start();
        bool isSafe = true;
        char st_tmp[30] = { 0 };
        QString q_st;
        int isWarn;
        int counter = 0;
        while (true)
        {
            if (scalpel::sharedM::GetInfoFromSharedMem(hSemaphore, ms_time, lpBase, &recvInfo, structSize))
            {
                isWarn = 0;
                //printf("%d: [oper %d]\n", ++counter,recvInfo.operation);
                isSafe = inspect(inspectRET,isWarn);
                if(!isSafe)
                {
                    sprintf(st_tmp, "(%d-%d-%d %02d:%02d:%02d %03d)", recvInfo.st.wYear, recvInfo.st.wMonth, recvInfo.st.wDay, recvInfo.st.wHour,
                        recvInfo.st.wMinute, recvInfo.st.wSecond, recvInfo.st.wMilliseconds);
                    //q_st = QString::fromLocal8Bit(st_tmp);
                    q_st = QString::fromUtf8(st_tmp);
                    if (isWarn)
                        emit messageWarn(inspectRET, q_st);
                    else
                        emit messageHint(inspectRET, q_st);
                }
                if (isWarn || filter(recvInfo.operation))
                {
                    bool ispass = filter(recvInfo.operation);
                    qDebug() << "isWarn: " << isWarn << " "<< ispass << endl;
                    visualization();
                    emit updateInfo(q_st);
                }
                emit updateStatisticsInfo(recvInfo.operation, isWarn);
                ReleaseSemaphore(hLockerSemaphore, 1, NULL);
            }
            if (WaitForSingleObject(pi.hProcess, 1) == WAIT_OBJECT_0)
            {
                if(processState)
                    emit injectedProcessStopSig();
                break;
            }
        }
    }
    else
    {
        CloseHandle(hMapFile);
        CloseHandle(hSemaphore);
        //qDebug() << "wrong" << endl;
        emit messageBoxInfo("提示", "程序运行错误");
        return;
    }
    quit();
}
QString Widget::operNum2Str(int num)
{
    QString str;
    switch (num)
    {
        case MESSAGEBOX_A :
            str = "MessageBoxA";break;
        case MESSAGEBOX_W :
            str = "MessageBoxW";break;
        case HEAPCREATE :
            str = "HeapCreate";break;
        case HEAPDESTROY :
            str = "HeapDestroy";break;
        case HEAPALLOC :
            str = "HeapAlloc";break;
        case HEAPFREE :
            str = "HeapFree";break;
        case CREATEFILE_A :
            str = "CreateFileA";break;
        case CREATEFILE_W :
            str = "CreateFileW";break;
        case WRITEFILE :
            str = "WriteFile";break;
        case READFILE :
            str = "ReadFile";break;
        case LCLOSE :
            str = "_lcose";break;
        case CLOSEHANDLE :
            str = "CloseHandle";break;
        case COPYFILE_A:
            str = "CopyFileA"; break;
        case REGCREATEKEYEX_A :
            str = "RegCreateKeyExA";break;
        case REGDELETEKEY_A:
            str = "RegDeleteKeyA"; break;
        case REGOPENKEYEX_A :
            str = "RegOpenKey";break;
        case REGCLOSEKEY :
            str = "RegCloseKey";break;
        case REGSETKEYVALUE_A :
            str = "RegSetKeyValue";break;
        case REGDELETEVALUE_A :
            str = "RegDeleteValueA";break;
        case REGSETVALUEEX_A:
            str = "RegSetValueExA"; break;
        case REGSETVALUE_A:
            str = "RegSetValueA";break;
        case REGOPENKEY_A:
            str = "RegOpenKeyA";break;
        case REGQUERYVALUE_A:
            str = "RegQueryValueA"; break;
        case REGCREATEKEY_A:
            str = "RegCreateKeyA"; break;
        case NEWSOCKET :
            str = "socket";break;
        case NEWSEND :
            str = "send";break;
        case NEWRECV :
            str = "recv";break;
        case NEWCONNECT :
            str = "connect";break;
        case NEWBIND :
            str = "bind";break;
        case NEWMEMCPY:
            str = "memcpy"; break;
        case NEWMEMMOVE:
            str = "memmove"; break;
        default: str="unknown";break;
    }
    return str;
}
void scalpelInjectThread::visualization()
{
    //scalpel::sharedM::hookedInfo* lp = (scalpel::sharedM::hookedInfo*)lpBase;
    switch (recvInfo.operation)
    {
        // MessageBoxA/W
        case MESSAGEBOX_A:
        case MESSAGEBOX_W:
            //MessageBoxW()
            sprintf((char *)recvInfo.param_val[0], "%08x", *((UINT*)(recvInfo.param_val[0])));
            sprintf((char*)recvInfo.param_val[2], "%08x", *((UINT*)(recvInfo.param_val[2])));
            sprintf((char *)recvInfo.param_val[3], "%08x", *((UINT*)(recvInfo.param_val[3])));
            break;
        case HEAPCREATE:
        case HEAPALLOC:
        case HEAPFREE:
            sprintf((char*)recvInfo.param_val[0], "%08x", *((UINT*)(recvInfo.param_val[0])));
            sprintf((char*)recvInfo.param_val[1], "%08x", *((UINT*)(recvInfo.param_val[1])));
            sprintf((char*)recvInfo.param_val[2], "%08x", *((UINT*)(recvInfo.param_val[2])));
            break;
        case HEAPDESTROY:
            sprintf((char*)recvInfo.param_val[0], "%08x", *((UINT*)(recvInfo.param_val[0])));
            break;
        case CREATEFILE_A:
        case CREATEFILE_W:
            //CreateFileA
            sprintf((char*)recvInfo.param_val[1], "%08x", *((UINT*)(recvInfo.param_val[1])));
            sprintf((char*)recvInfo.param_val[2], "%08x", *((UINT*)(recvInfo.param_val[2])));
            sprintf((char*)recvInfo.param_val[3], "%08x", *((UINT*)(recvInfo.param_val[3])));
            sprintf((char*)recvInfo.param_val[4], "%08x", *((UINT*)(recvInfo.param_val[4])));
            sprintf((char*)recvInfo.param_val[5], "%08x", *((UINT*)(recvInfo.param_val[5])));
            sprintf((char*)recvInfo.param_val[6], "%08x", *((UINT*)(recvInfo.param_val[6])));
            break;
        case READFILE:
        case WRITEFILE:
            sprintf((char*)recvInfo.param_val[0], "%08x", *((UINT*)(recvInfo.param_val[0])));
            sprintf((char*)recvInfo.param_val[2], "%08x", *((UINT*)(recvInfo.param_val[2])));
            sprintf((char*)recvInfo.param_val[3], "%08x", *((UINT*)(recvInfo.param_val[3])));
            sprintf((char*)recvInfo.param_val[4], "%08x", *((UINT*)(recvInfo.param_val[4])));
            break;
        case LCLOSE:
        case CLOSEHANDLE:
            sprintf((char*)recvInfo.param_val[0], "%08x", *((UINT*)(recvInfo.param_val[0])));
            break;
        case COPYFILE_A:
            sprintf((char*)recvInfo.param_val[2], "%08x", *((UINT*)(recvInfo.param_val[2])));
            break;
        case REGCREATEKEYEX_A:
            //RegCreateKeyExA()
            sprintf((char*)recvInfo.param_val[0], "%08x", *((UINT*)(recvInfo.param_val[0])));
            sprintf((char*)recvInfo.param_val[2], "%08x", *((UINT*)(recvInfo.param_val[2])));
            //sprintf((char*)recvInfo.param_val[3], "%08x", *((UINT*)(recvInfo.param_val[3])));
            sprintf((char*)recvInfo.param_val[4], "%08x", *((UINT*)(recvInfo.param_val[4])));
            sprintf((char*)recvInfo.param_val[5], "%08x", *((UINT*)(recvInfo.param_val[5])));
            sprintf((char*)recvInfo.param_val[6], "%08x", *((UINT*)(recvInfo.param_val[6])));
            sprintf((char*)recvInfo.param_val[7], "%08x", *((UINT*)(recvInfo.param_val[7])));
            sprintf((char*)recvInfo.param_val[8], "%08x", *((UINT*)(recvInfo.param_val[8])));
        case REGOPENKEYEX_A:
            //RegOpenKeyExA()
            sprintf((char*)recvInfo.param_val[0], "%08x", *((UINT*)(recvInfo.param_val[0])));
            sprintf((char*)recvInfo.param_val[2], "%08x", *((UINT*)(recvInfo.param_val[2])));
            sprintf((char*)recvInfo.param_val[3], "%08x", *((UINT*)(recvInfo.param_val[3])));
            sprintf((char*)recvInfo.param_val[4], "%08x", *((UINT*)(recvInfo.param_val[4])));
            break;
        case REGQUERYVALUE_A:
            sprintf((char*)recvInfo.param_val[0], "%08x", *((UINT*)(recvInfo.param_val[0])));
            sprintf((char*)recvInfo.param_val[3], "%08x", *((UINT*)(recvInfo.param_val[3])));
            break;
        case REGCLOSEKEY:
        case REGDELETEVALUE_A:
        case REGDELETEKEY_A:
            sprintf((char*)recvInfo.param_val[0], "%08x", *((UINT*)(recvInfo.param_val[0])));
            break;
        case REGSETKEYVALUE_A:
            //RegSetKeyValueA()
            sprintf((char*)recvInfo.param_val[0], "%08x", *((UINT*)(recvInfo.param_val[0])));
            sprintf((char*)recvInfo.param_val[3], "%08x", *((UINT*)(recvInfo.param_val[3])));
            sprintf((char*)recvInfo.param_val[5], "%08x", *((UINT*)(recvInfo.param_val[5])));
            break;
        case REGSETVALUEEX_A:
            //RegSetValueExA()
            sprintf((char*)recvInfo.param_val[0], "%08x", *((UINT*)(recvInfo.param_val[0])));
            sprintf((char*)recvInfo.param_val[2], "%08x", *((UINT*)(recvInfo.param_val[2])));
            sprintf((char*)recvInfo.param_val[3], "%08x", *((UINT*)(recvInfo.param_val[3])));
            sprintf((char*)recvInfo.param_val[5], "%08x", *((UINT*)(recvInfo.param_val[5])));
            break;
        case REGCREATEKEY_A:
        case REGOPENKEY_A:
            //RegOpenKeyA()
            //RegCreateKeyA
            sprintf((char*)recvInfo.param_val[0], "%08x", *((UINT*)(recvInfo.param_val[0])));
            sprintf((char*)recvInfo.param_val[2], "%08x", *((UINT*)(recvInfo.param_val[2])));
            break;
        case REGSETVALUE_A:
            //RegSetValueA()
            sprintf((char*)recvInfo.param_val[0], "%08x", *((UINT*)(recvInfo.param_val[0])));
            sprintf((char*)recvInfo.param_val[2], "%08x", *((UINT*)(recvInfo.param_val[2])));
            sprintf((char*)recvInfo.param_val[4], "%08x", *((UINT*)(recvInfo.param_val[4])));
            break;
            // socket send recv connect bind
        case NEWSOCKET:
            sprintf((char*)recvInfo.param_val[0], "%08x", *((UINT*)(recvInfo.param_val[0])));
            sprintf((char*)recvInfo.param_val[1], "%08x", *((UINT*)(recvInfo.param_val[1])));
            sprintf((char*)recvInfo.param_val[2], "%08x", *((UINT*)(recvInfo.param_val[2])));
            break;
        case NEWSEND:
        case NEWRECV:
            sprintf((char*)recvInfo.param_val[0], "%08x", *((UINT*)(recvInfo.param_val[0])));
            sprintf((char*)recvInfo.param_val[1], "%08x", *((UINT*)(recvInfo.param_val[1])));
            sprintf((char*)recvInfo.param_val[2], "%08x", *((UINT*)(recvInfo.param_val[2])));
            sprintf((char*)recvInfo.param_val[3], "%08x", *((UINT*)(recvInfo.param_val[3])));
            break;
        case NEWBIND:
        case NEWCONNECT:
            sprintf((char*)recvInfo.param_val[0], "%08x", *((UINT*)(recvInfo.param_val[0])));
            sprintf((char*)recvInfo.param_val[2], "%08x", *((UINT*)(recvInfo.param_val[3])));
            sprintf((char*)recvInfo.param_val[3], "%08x", *((UINT*)(recvInfo.param_val[2])));
            break;
        case NEWMEMCPY:
        case NEWMEMMOVE:
            sprintf((char*)recvInfo.param_val[0], "%08x", *((UINT*)(recvInfo.param_val[2])));
            break;
        default:
            break;
    }
    return;
}
void Widget::injectedThreadUpateInfo(QString tmp)
{
    QTreeWidgetItem* item = new QTreeWidgetItem();
    item->setData(0,0,operNum2Str(injector.recvInfo.operation)+tmp);
    //QTreeWidgetItem* item3 = new QTreeWidgetItem();
    //item3->setData(0, 0, "参数名称");
    //item3->setData(1, 0, "参数值");
    //item->addChild(item3);
    for (int i = 0; i < injector.recvInfo.param_num; i++)
    {
        QTreeWidgetItem* item2 = new QTreeWidgetItem();
        item2->setData(0, 0, (char *)injector.recvInfo.param_name[i]);
        item2->setData(1, 0, (char *)injector.recvInfo.param_val[i]);
        item->addChild(item2);
    }
    if (injector.recvInfo.payload_len > 0)
    {
        QTreeWidgetItem* item3 = new QTreeWidgetItem();
        if(scalpel::judge::CipherJudger((char*)injector.recvInfo.payload))
            item3->setData(0, 0, "负载(密文)");
        else item3->setData(0, 0, "负载(明文)");
        item3->setData(1, 0, (char *)injector.recvInfo.payload);
        item->addChild(item3);
    }
    QTreeWidgetItem* item4 = new QTreeWidgetItem();
    item4->setData(0, 0, "返回值");
    char ret_str[9]={0};
    sprintf(ret_str, "%08x", *((UINT*)(injector.recvInfo.ret_val)));
    item4->setData(1, 0, ret_str);
    item->addChild(item4);
    ui->processScreen->addTopLevelItem(item);
    ui->processScreen->scrollToBottom();
    return;
}
void Widget::injectedThreadUpateStatisticsInfo(char ch_title, bool isWarn)
{
    QString title = operNum2Str(ch_title);
    QTreeWidgetItemIterator iter(ui->totalScreen);
    int cnt = 0;
    while (*iter)
    {
        QString funcName = (*iter)->data(0,0).toString();
        if (funcName == title)
        {
            cnt = (*iter)->data(1, 0).toInt() + 1;
            (*iter)->setData(1, 0, cnt); //total
            if (isWarn)
            {
                cnt = (*iter)->data(2, 0).toInt() + 1;
                (*iter)->setData(2, 0, cnt); //isWarn
            }
            break;
        }
        iter++;
    }
    if (cnt == 0)
    {
        QTreeWidgetItem* item = new QTreeWidgetItem();
        item->setData(0, 0, title);
        item->setData(1, 0, 1);
        if (isWarn)
            item->setData(2, 0, 1);
        else
            item->setData(2, 0, 0);
        ui->totalScreen->addTopLevelItem(item);
    }
    return;
}
void Widget::initScreen()
{
    ui->processScreen->clear();
    QString param = "截获的函数";
    QString ret = "";
    ui->processScreen->setHeaderLabels(QStringList() << param << ret << ret);

    ui->totalScreen->clear();
    QString param1 = "截获的函数";
    QString l1 = "总调用次数";
    QString l2 = "异常次数";
    ui->totalScreen->setHeaderLabels(QStringList() << param1 << l1 <<l2);

    ui->message->clear();
    //ui->message->clear();
    return;
}
bool scalpelInjectThread::inspect(QString &ret,int &isWarn)
{
    bool isSafe = true;
    char oper = recvInfo.operation;
    if (oper <= MESSAGEBOX_W)
    {
        return isSafe;
    }
    else if (oper <= HEAPFREE)
    {
        isSafe = scalpel::judge::heapBehaviorJudge(recvInfo, ret, isWarn);
    }
    else if (oper <= COPYFILE_A)
    {
        isSafe = scalpel::judge::fileBehaviorJudge(recvInfo,receptorPath, receptorName, ret, isWarn);
    }
    else if (oper <= REGCREATEKEY_A)
    {
        isSafe = scalpel::judge::regBehaviorJudge(recvInfo, ret ,isWarn);
    }
    else if (oper <= NEWBIND)
    {
        isSafe = scalpel::judge::netBehaviorTrace(recvInfo, ret, isWarn);
    }
    return isSafe;
}
void Widget::injectedThreadHint(QString hint, QString st)
{
    QTreeWidgetItem* item = new QTreeWidgetItem();
    //qDebug() << "hint: " << hint << endl;
    item->setTextColor(0, QColor(0, 47, 167, 255));
    item->setText(0, hint);
    QTreeWidgetItem* item2 = new QTreeWidgetItem();
    item2->setData(0, 0, st);
    item->addChild(item2);
    ui->message->addTopLevelItem(item);
    ui->message->scrollToBottom();
    return;
}
void Widget::injectedThreadWarn(QString warn,QString st)
{
    QTreeWidgetItem* item = new QTreeWidgetItem();
    //qDebug() << "warn: " << warn << endl;
    //item->setTextColor(0, QColor(219, 93, 107, 255));
    item->setTextColor(0, QColor(0xc9, 0x37, 0x56, 255));

    item->setText(0, warn);
    QTreeWidgetItem* item2 = new QTreeWidgetItem();
    item2->setData(0, 0, st);
    item->addChild(item2);
    ui->message->addTopLevelItem(item);
    ui->message->scrollToBottom();
    return;
}
void emunModule::run()
{
    msleep(1200);
    HMODULE hMods[128 * sizeof(HANDLE)] = { 0 };
    DWORD cbNeeded;
    int moduleNum;
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dwProcessId);
    if (NULL == hProcess)
    {
        moduleStr = "获取失败: ";
        moduleStr += QString::number(GetLastError());
        emit updateModule(moduleStr);
        return;
    }

    char szModName[MAX_PATH];
    if (EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded))
    {
        moduleNum = (cbNeeded / sizeof(HMODULE));
        moduleStr = "载入模块数量: " + QString::number(moduleNum);
        emit updateModule(moduleStr);
        for (int i = 0; i < moduleNum; i++)
        {
            // Get the full path to the module's file.
            memset(szModName, 0, MAX_PATH);
            if (GetModuleBaseNameA(hProcess, hMods[i], szModName, sizeof(szModName) / sizeof(char)))
            {
                //moduleStr = QString::fromLocal8Bit(szModName);
                moduleStr = QString::fromUtf8(szModName);
                emit updateModule(moduleStr);

            }
        }
    }
    else
    {
        moduleStr = "获取失败: ";
        moduleStr += QString::number(GetLastError());
        emit updateModule(moduleStr);
    }
}
bool scalpelInjectThread::filter(uint8_t op)
{
    bool ret = true;
    if (op <= MESSAGEBOX_W)
    {
        return true;
    }
    else if (op <= HEAPFREE)
    {
        if (heapFilter)ret = false;
    }
    else if (op <= COPYFILE_A)
    {
        if (fileFilter)ret = false;
    }
    else if (op <= REGCREATEKEY_A)
    {
        if (regFilter)ret = false;
    }
    else if (op <= NEWBIND)
    {
        if (netFilter)ret = false;
    }
    return ret;
}