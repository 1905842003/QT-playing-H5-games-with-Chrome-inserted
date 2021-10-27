#ifndef DIALOG_SETTITLE_H
#define DIALOG_SETTITLE_H

#include <QDialog>

namespace Ui {
class Dialog_SetTitle;
}

class Dialog_SetTitle : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_SetTitle(QWidget *parent = nullptr);
    ~Dialog_SetTitle();

private slots:
    void on_pushButton_apply_clicked();

private:
    Ui::Dialog_SetTitle *ui;
};

#endif // DIALOG_SETTITLE_H
