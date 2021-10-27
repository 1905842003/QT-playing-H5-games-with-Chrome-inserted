#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <Windows.h>
#include <QFileInfo>
#include <QSettings>
#include <QTextCodec>
#include <QTimer>
#include <QMessageBox>
#include <QDir>
#include "dialog_settitle.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void readConfig();
    void writeConfig();

private:
    Ui::MainWindow *ui;
    HWND H5_hwnd;
    int CXFRAME,CYFRAME,CYCAPTION;
    int width,height;
    int accountGroup;
    QTimer *timer_1;
    QString url;

protected:
    bool eventFilter(QObject *watched, QEvent *event);

private slots:
    void resizeH5();
    void afterConstruct();
    void on_action_720p_triggered();
    void on_action_accountGroup2_triggered();
    void on_action_accountGroup1_triggered();
    void on_action_accountGroup3_triggered();
    void on_action_360p_triggered();
    void on_action_480p_triggered();
    void on_action_900p_triggered();
    void on_action_1080p_triggered();
    void on_action_clearCache_triggered();
    void on_action_clearData_triggered();
    void on_action_setTitle_triggered();
};
#endif // MAINWINDOW_H
