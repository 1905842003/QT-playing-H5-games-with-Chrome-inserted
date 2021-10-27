#include "dialog_settitle.h"
#include "ui_dialog_settitle.h"
#include "mainwindow.h"

Dialog_SetTitle::Dialog_SetTitle(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_SetTitle)
{
    Qt::WindowFlags flags= this->windowFlags();
    setWindowFlags(flags&~Qt::WindowContextHelpButtonHint);
    ui->setupUi(this);
    MainWindow *ptr=(MainWindow*)parentWidget();
    ui->lineEdit_title->setText(ptr->windowTitle());
}

Dialog_SetTitle::~Dialog_SetTitle()
{
    delete ui;
}

void Dialog_SetTitle::on_pushButton_apply_clicked()
{
    MainWindow *ptr=(MainWindow*)parentWidget();
    if(ui->lineEdit_title->text().trimmed().length()>0){
        ptr->setWindowTitle(ui->lineEdit_title->text().trimmed());
        ptr->writeConfig();
    }
}
