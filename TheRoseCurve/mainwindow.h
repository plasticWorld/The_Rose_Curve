#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qcustomplot.h"


namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

    public:
    explicit MainWindow (QWidget *parent = 0);
    ~MainWindow();

    private slots:
    void on_BuildButton_clicked();
    void on_DefaultButton_clicked();
	void slotTimer();
	void on_checkBox_clicked(bool checked);

private:
    int pointCount = 0;
    int timeElapsed;
    int key = 0;
    int A;
    int B;
    int C;
    Ui::MainWindow *ui;
    QVector<QCPCurve *> rounds;
    QMap<int, QVector<QCPCurveData>> pointsOfGrid;
    QTimer *timer = new QTimer (this);
    QCPItemTracer *curveTracer;
    QCPCurve *curveByPoints;
    QVector<double> x2/*(pointCount)*/, y2/*(pointCount)*/;
    int setPointCount();
    void setupRoseCurve (QCustomPlot *widget);
    void setupDefaultValues();
    void areaCleaner();
    void forGridView (QCustomPlot *widget);
};

#endif // MAINWINDOW_H
