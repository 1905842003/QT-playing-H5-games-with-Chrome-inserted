#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->readConfig();
    QString path=QCoreApplication::applicationDirPath()+"//数据//用户组"+QString::number(this->accountGroup);
    QString strParameter="--app="+this->url+" --disk-cache-dir="+path+" --user-data-dir="+path;
    HINSTANCE hInstance=ShellExecute(NULL,L"open",L"chrome",strParameter.toStdWString().c_str(),NULL,SW_NORMAL);
    if((int)hInstance<32){//未安装Chrome时，提示安装
        QMessageBox::StandardButton result=QMessageBox::information(NULL,"提示","请下载Chrome浏览器并重启本微端，是否立即打开Chrome浏览器下载页面?",QMessageBox::Yes|QMessageBox::No);
        switch(result)
        {
        case QMessageBox::Yes:{
            ShellExecute(NULL,L"open",L"https://www.google.cn/chrome/",L"",L"",SW_SHOWNORMAL);
            break;
        }
        case QMessageBox::No:
            break;
        default:
            break;
        }
    }
    ::Sleep(1000);//给Chrome启动预留时间，以便成功获取句柄
    H5_hwnd=::FindWindow(L"Chrome_WidgetWin_1",NULL);//获取句柄
    ::SetParent(H5_hwnd,(HWND)ui->groupBox->winId());//将Chrome窗口嵌入groupBox
    CXFRAME=GetSystemMetrics(SM_CXFRAME);//边框宽度
    CYFRAME=GetSystemMetrics(SM_CYFRAME);//边框宽度
    CYCAPTION=GetSystemMetrics(SM_CYCAPTION);//标题栏高度
    ::SetWindowPos(H5_hwnd,NULL,-CXFRAME,-CYFRAME-CYCAPTION,this->width+2*CXFRAME,this->height+CYCAPTION+2*CYFRAME,NULL);//调整Chrome窗口的大小与位置
    switch(this->accountGroup){
    case 1:ui->action_accountGroup1->setChecked(true);break;
    case 2:ui->action_accountGroup2->setChecked(true);break;
    case 3:ui->action_accountGroup3->setChecked(true);break;
    default:break;
    }
    this->installEventFilter(this);
    this->timer_1=new QTimer(this);
    this->timer_1->start(1000);
    connect(this->timer_1,SIGNAL(timeout()),this,SLOT(afterConstruct()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * @brief MainWindow::afterConstruct
 * MainWindow构造完成后才能正确获取statusBar的高度
 */
void MainWindow::afterConstruct(){
    this->timer_1->stop();
    this->timer_1->disconnect();
    this->resize(this->width,this->height+this->menuBar()->height()+this->statusBar()->height());
}

/**
 * @brief MainWindow::resizeH5
 * 根据groupBox新大小调整游戏主体大小，并将groupBox大小写入ini
 */
void MainWindow::resizeH5(){
    ::SetWindowPos(H5_hwnd,NULL,-CXFRAME,-CYFRAME-CYCAPTION,ui->groupBox->width()+2*CXFRAME,ui->groupBox->height()+CYCAPTION+2*CYFRAME,NULL);
    this->width=ui->groupBox->width();
    this->height=ui->groupBox->height();
    this->writeConfig();
}

/**
 * @brief MainWindow::eventFilter
 * 窗口大小调整完成时，调用resizeH5()
 * @param watched
 * @param event
 * @return
 */
bool MainWindow::eventFilter(QObject *watched, QEvent *event){
    if(watched==this&&event->type()==QEvent::HoverEnter){
        this->resizeH5();
    }
    return QMainWindow::eventFilter(watched, event);
}

/**
 * @brief MainWindow::readConfig
 * 读取ini
 */
void MainWindow::readConfig(){
    QFileInfo fileInfo("数据//SeerH5Config.ini");
    if(!fileInfo.isFile()){
        QSettings settings("数据//SeerH5Config.ini",QSettings::IniFormat);
        settings.setIniCodec(QTextCodec::codecForName("GB2312"));
        settings.beginGroup("Caution");
        settings.setValue("Code","GB2312");
        settings.endGroup();
        settings.beginGroup("Settings");
        settings.setValue("Title","赛尔号互通版");
        settings.setValue("Width",1280);
        settings.setValue("Height",720);
        settings.setValue("AccountGroup",1);
        settings.setValue("URL","http://seerh5.61.com");
        settings.endGroup();
        this->accountGroup=1;
        this->width=1280;
        this->height=720;
        this->url="http://seerh5.61.com";
    }
    else{
        QSettings settings("数据//SeerH5Config.ini",QSettings::IniFormat);
        settings.setIniCodec(QTextCodec::codecForName("GB2312"));
        settings.beginGroup("Settings");
        this->setWindowTitle(settings.value("Title").toString());
        this->width=settings.value("Width").toInt();
        this->height=settings.value("Height").toInt();
        this->accountGroup=settings.value("AccountGroup").toInt();
        this->url=settings.value("URL").toString();
        settings.endGroup();
    }
}

/**
 * @brief MainWindow::writeConfig
 * 写入ini
 */
void MainWindow::writeConfig(){
    QSettings settings("数据//SeerH5Config.ini",QSettings::IniFormat);
    settings.setIniCodec(QTextCodec::codecForName("GB2312"));
    settings.beginGroup("Caution");
    settings.setValue("Code","GB2312");
    settings.endGroup();
    settings.beginGroup("Settings");
    settings.setValue("Title",this->windowTitle());
    settings.setValue("Width",this->width);
    settings.setValue("Height",this->height);
    settings.setValue("AccountGroup",this->accountGroup);
    settings.setValue("URL",this->url);
    settings.endGroup();
}

void MainWindow::on_action_accountGroup1_triggered()
{
    this->accountGroup=1;
    this->writeConfig();
    ::CloseWindow(this->H5_hwnd);
    QString path=QCoreApplication::applicationDirPath()+"//数据//用户组"+QString::number(this->accountGroup);
    QString strParameter="--app="+this->url+" --disk-cache-dir="+path+" --user-data-dir="+path;
    ShellExecute(NULL,L"open",L"chrome",strParameter.toStdWString().c_str(),NULL,SW_NORMAL);
    ::Sleep(1000);
    H5_hwnd=::FindWindow(L"Chrome_WidgetWin_1",NULL);
    ::SetParent(H5_hwnd,(HWND)ui->groupBox->winId());
    ::SetWindowPos(H5_hwnd,NULL,-CXFRAME,-CYFRAME-CYCAPTION,this->width+2*CXFRAME,this->height+CYCAPTION+2*CYFRAME,NULL);
    ui->action_accountGroup1->setChecked(true);
    ui->action_accountGroup2->setChecked(false);
    ui->action_accountGroup3->setChecked(false);
}

void MainWindow::on_action_accountGroup2_triggered()
{
    this->accountGroup=2;
    this->writeConfig();
    ::CloseWindow(this->H5_hwnd);
    QString path=QCoreApplication::applicationDirPath()+"//数据//用户组"+QString::number(this->accountGroup);
    QString strParameter="--app="+this->url+" --disk-cache-dir="+path+" --user-data-dir="+path;
    ShellExecute(NULL,L"open",L"chrome",strParameter.toStdWString().c_str(),NULL,SW_NORMAL);
    ::Sleep(1000);
    H5_hwnd=::FindWindow(L"Chrome_WidgetWin_1",NULL);
    ::SetParent(H5_hwnd,(HWND)ui->groupBox->winId());
    ::SetWindowPos(H5_hwnd,NULL,-CXFRAME,-CYFRAME-CYCAPTION,this->width+2*CXFRAME,this->height+CYCAPTION+2*CYFRAME,NULL);
    ui->action_accountGroup1->setChecked(false);
    ui->action_accountGroup2->setChecked(true);
    ui->action_accountGroup3->setChecked(false);
}

void MainWindow::on_action_accountGroup3_triggered()
{
    this->accountGroup=3;
    this->writeConfig();
    ::CloseWindow(this->H5_hwnd);
    QString path=QCoreApplication::applicationDirPath()+"//数据//用户组"+QString::number(this->accountGroup);
    QString strParameter="--app="+this->url+" --disk-cache-dir="+path+" --user-data-dir="+path;
    ShellExecute(NULL,L"open",L"chrome",strParameter.toStdWString().c_str(),NULL,SW_NORMAL);
    ::Sleep(1000);
    H5_hwnd=::FindWindow(L"Chrome_WidgetWin_1",NULL);
    ::SetParent(H5_hwnd,(HWND)ui->groupBox->winId());
    ::SetWindowPos(H5_hwnd,NULL,-CXFRAME,-CYFRAME-CYCAPTION,this->width+2*CXFRAME,this->height+CYCAPTION+2*CYFRAME,NULL);
    ui->action_accountGroup1->setChecked(false);
    ui->action_accountGroup2->setChecked(false);
    ui->action_accountGroup3->setChecked(true);
}

void MainWindow::on_action_360p_triggered()
{
    this->resize(640,360+this->menuBar()->height()+this->statusBar()->height());
    this->resizeH5();
    ui->action_360p->setChecked(true);
    ui->action_480p->setChecked(false);
    ui->action_720p->setChecked(false);
    ui->action_900p->setChecked(false);
    ui->action_1080p->setChecked(false);
}

void MainWindow::on_action_480p_triggered()
{
    this->resize(960,540+this->menuBar()->height()+this->statusBar()->height());
    this->resizeH5();
    ui->action_360p->setChecked(false);
    ui->action_480p->setChecked(true);
    ui->action_720p->setChecked(false);
    ui->action_900p->setChecked(false);
    ui->action_1080p->setChecked(false);
}

void MainWindow::on_action_720p_triggered()
{
    this->resize(1280,720+this->menuBar()->height()+this->statusBar()->height());
    this->resizeH5();
    ui->action_360p->setChecked(false);
    ui->action_480p->setChecked(false);
    ui->action_720p->setChecked(true);
    ui->action_900p->setChecked(false);
    ui->action_1080p->setChecked(false);
}

void MainWindow::on_action_900p_triggered()
{
    this->resize(1600,900+this->menuBar()->height()+this->statusBar()->height());
    this->resizeH5();
    ui->action_360p->setChecked(false);
    ui->action_480p->setChecked(false);
    ui->action_720p->setChecked(false);
    ui->action_900p->setChecked(true);
    ui->action_1080p->setChecked(false);
}

void MainWindow::on_action_1080p_triggered()
{
    this->resize(1920,1080+this->menuBar()->height()+this->statusBar()->height());
    this->resizeH5();
    ui->action_360p->setChecked(false);
    ui->action_480p->setChecked(false);
    ui->action_720p->setChecked(false);
    ui->action_900p->setChecked(false);
    ui->action_1080p->setChecked(true);
}

/**
 * @brief MainWindow::on_action_clearCache_triggered
 * 会导致游戏异常，刷新后无法进入游戏
 */
void MainWindow::on_action_clearCache_triggered()
{
    QDir dir(QCoreApplication::applicationDirPath()+"//数据//用户组"+QString::number(this->accountGroup)+"//Default//Cache");
    dir.removeRecursively();
    dir.setPath(QCoreApplication::applicationDirPath()+"//数据//用户组"+QString::number(this->accountGroup)+"//Default//Code Cache");
    dir.removeRecursively();
}

/**
 * @brief MainWindow::on_action_clearData_triggered
 * 会导致游戏异常，刷新后无法进入游戏
 */
void MainWindow::on_action_clearData_triggered()
{
    QMessageBox::StandardButton result=QMessageBox::information(NULL,"提示","将清空当前用户组的所有数据，是否继续？",QMessageBox::Yes|QMessageBox::No);
    switch(result)
    {
    case QMessageBox::Yes:{
        QDir dir(QCoreApplication::applicationDirPath()+"//数据//用户组"+QString::number(this->accountGroup));
        dir.removeRecursively();
        break;
    }
    case QMessageBox::No:
        break;
    default:
        break;
    }
}

void MainWindow::on_action_setTitle_triggered()
{
    Dialog_SetTitle d(this);
    d.show();
    d.exec();
}
