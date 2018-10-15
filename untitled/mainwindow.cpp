#include "mainwindow.h"
#include "ui_mainwindow.h"

void PlaybackStep()
{
    int TimeElapsed = 0;
    QVector<double> x2, y2;
    TimeElapsed+=50; // 50 - частота срабатывания таймера (в мс)
    for(int i = 0; i < x.size(); i++)
    {
            if(TimeElapsed >= x[i])
            {
                x2.push_back(x[i]);
                y2.push_back(y[i]);
                x.pop_front();
                y.pop_front();
                i = 0; // если во временном промежутке несколько подходящих "точек", то после pop_front() мы можем
                // упустить одну. i = 0 запускает заново цикл, чтобы ничего "не потерять"
            }
    }
    ui->widget->graph(0)->setData(x2, y2);
    //end of playback check
    if(x.size() == 0) playBackTimer->stop();
    //
    ui->widget->replot();
}



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // generate some data:
    int TimeElapsed = 0;

    QVector<double> x(100), y(100);

    QTimer* playBackTimer = new QTimer(this); //создание экземпляра таймера
    connect(playBackTimer, SIGNAL(timeout()), this, SLOT(PlaybackStep())); //привязка события к функции PlaybackStep()

    playBackTimer->start(50);

    for(int i = 0; i < 100; i++)
    {
       x[i] = 100*i;
       y[i] = 0.5*x[i];
    }
    // create graph and assign data to it:
    ui->widget->addGraph();
    ui->widget->graph(0)->setData(x, y);

    ui->widget->xAxis->setRange(0, 10000);
    ui->widget->yAxis->setRange(0, 10000);
    ui->widget->replot();
}

MainWindow::~MainWindow()
{
    delete ui;
}
