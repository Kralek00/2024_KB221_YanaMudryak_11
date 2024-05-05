#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QtCore>
#include <QtGui>
#include <QDir>
#include <QDirModel>
#include <QInputDialog>
#include <QFileSystemModel>
#include <QMessageBox>


namespace Ui {
class Dialog;
}

class Dialog : public QDialog {
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private slots:
    void on_pushButton_6_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

private:
    Ui::Dialog *ui;
    QDirModel *model;
    QFileSystemModel *dirmodel;
    QFileSystemModel *filemodel;

    QString fileNameCopy;
    QString path;


};

#endif // DIALOG_H
