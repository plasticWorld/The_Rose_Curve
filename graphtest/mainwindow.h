#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qcustomplot.h"
#include <vector>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    double A = 0;
    double range=0;
    QVector<double> allA;

    ~MainWindow();

private slots:
   void on_BuildButton_clicked();

   void on_ClearButton_clicked();

   void on_checkBox_clicked();

   void on_AFactor_valueChanged(int arg1);

   void on_DefaultButton_clicked();

private:
    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H
