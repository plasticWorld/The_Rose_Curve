#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <vector>
#include <algorithm>
#include <QString>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //this->setGeometry(100,100,700,400);//размеры окна
    ui->widget->hide();

    ui->RCurveSlider->setValue(190);
    ui->GCurveSlider->setValue(85);
    ui->BCurveSlider->setValue(85);
    ui->AFactor->setValue(1);
    ui->BFactor->setValue(2);
    ui->CFactor->setValue(7);
    ui->SizeLineSlider->setValue(2);

}

MainWindow::~MainWindow()
{
    delete ui;

}

void MainWindow::on_BuildButton_clicked()
{
     ui->widget->show();

if (ui->checkBoxAuto->isChecked()){
    ui->widget->clearPlottables();
    allA.clear();
}

    QCPCurve *newCurve= new QCPCurve(ui->widget->xAxis,ui->widget->yAxis);

    int pointCount = 3600;

     QVector<double> x(pointCount), y(pointCount), x1(360), y1(360);

     double A = ui->AFactor->value()*1.0;
     double B = ui->BFactor->value()*1.0;
     double C = ui->CFactor->value()*1.0;
     allA.append(A);

    for (int i=0; i<pointCount; i++)
    {
      double theta = (i/180.0)*M_PI;
      x[i] = A*qSin((B/C)*theta)*qCos(1*theta);
      y[i]= A*qSin((B/C)*theta)*qSin(1*theta);
    }
    //Говорим, что отрисовать нужно график по нашим двум массивам x и y
    newCurve->setData(x, y);



      //  color the curves:

        int RLine = ui->RCurveSlider->value();
        int GLine = ui->GCurveSlider->value();
        int BLine = ui->BCurveSlider->value();

        int LineWidth = ui->SizeLineSlider->value();

        //настройка внешнего вида для линий кривой
        QPen penForCurve(QColor(RLine, GLine, BLine,255), LineWidth, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin);
        //newCurve->setRenderHint(QPainter::Antialiasing);
        newCurve->setPen(penForCurve);

        QPen penForGrid(QColor(100, 100, 100, 150), 1, Qt::DotLine, Qt::SquareCap, Qt::MiterJoin);

        QPainter *tracker = new QPainter(ui->widget);
        QPen penTracker(QColor(0, 0, 0), 1, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin);
        QBrush brushTracker (QBrush(QColor(255, 120, 0, 30)));
        tracker->setPen(penTracker);
        tracker->setBrush(brushTracker);

        QRectF rectangle(50, 50, 100, 100);

        QPainter painter(ui->widget);
        painter.setPen(QPen(QColor(79, 106, 25), 1, Qt::SolidLine,
                            Qt::FlatCap, Qt::MiterJoin));
        painter.setBrush(QColor(122, 163, 39));
        painter.drawEllipse(rectangle);
        painter.translate(185, 185);

    //шаг основных делений на оси
    QSharedPointer<QCPAxisTickerFixed> fixedTicker(new QCPAxisTickerFixed);
  ui->widget->xAxis->setTicker(fixedTicker);
  ui->widget->yAxis->setTicker(fixedTicker);
  fixedTicker->setTickStep(1.0); // tick step
  fixedTicker->setScaleStrategy(QCPAxisTickerFixed::ssNone); // and no scaling of the tickstep (like multiples or powers) is allowed

  double *r = std::max_element(std::begin(allA), std::end(allA));
  double k = *r;

if (ui->checkBox->isChecked()){
    ui->widget->xAxis->setRange(-10.8, 10.8);
    ui->widget->yAxis->setRange(-10.8, 10.8);
}else{
   if(A>range){
       range  = A * 1.2;
       //задаем границы графика по осям
       ui->widget->xAxis->setRange(range*(-1), range);
       ui->widget->yAxis->setRange(range*(-1), range);
   }else{
       ui->widget->xAxis->setRange(k *(-1.2), k*1.2);
       ui->widget->yAxis->setRange(k *(-1.2), k*1.2);
   }
}

QVector<QCPCurve*> rCurve;

for(int j=1;j<=k;j++){
    QCPCurve *rCurvej = new QCPCurve(ui->widget->xAxis, ui->widget->yAxis);

    for(int i = 0; i < 360; i++){
        double theta = (i/180.0)*M_PI;
        x1[i]=j*qCos(1*theta);
        y1[i]=j*qSin(1*theta);
    }
    rCurvej->setData(x1, y1);
    rCurve.push_back(rCurvej);

     rCurvej->setPen(penForGrid);
}

  //задаем смещение осей
  ui->widget->xAxis->setOffset(-185);
  ui->widget->yAxis->setOffset(-185);
  //задаем отступ от краев формы до границ области графика
  ui->widget->xAxis->setPadding(0);
  ui->widget->yAxis->setPadding(0);
  //не показывать на осях промежуточные деления
  ui->widget->xAxis->setSubTicks(0);
  ui->widget->yAxis->setSubTicks(0);
//  ui->widget->xAxis->setTicks(0);
//  ui->widget->yAxis->setTicks(0);
  //убрать подписи делений по оси у
  ui->widget->yAxis->setTickLabels(0);
  ui->widget->xAxis->setTickLabels(0);
//убрать линии сетки
  ui->widget->yAxis->grid()->setVisible(0);
  ui->widget->xAxis->grid()->setVisible(0);
//внешний вид линий осей
   ui->widget->xAxis->setBasePen (penForGrid);
   ui->widget->yAxis->setBasePen (penForGrid);
   //
   newCurve->setAntialiased (1);

//   int TimeElapsed = 0;
//   QVector<double> x2, y2;

//   QTimer* playBackTimer = new QTimer(this); //создание экземпляра таймера
//   connect(playBackTimer, SIGNAL(timeout()), this, SLOT(PlaybackStep())); //привязка события к функции PlaybackStep()

//   playBackTimer->start(50);

//   void PlaybackStep()
//   {
//       TimeElapsed+=50; // 50 - частота срабатывания таймера (в мс)
//       for(int i = 0; i < x.size(); i++)
//       {
//               if(TimeElapsed >= x[i])
//               {
//                   x2.push_back(x[i]);
//                   y2.push_back(y[i]);
//                   x.pop_front();
//                   y.pop_front();
//                   i = 0; // если во временном промежутке несколько подходящих "точек", то после pop_front() мы можем
//                   // упустить одну. i = 0 запускает заново цикл, чтобы ничего "не потерять"
//               }
//       }
//       newCurve->setData(x2, y2);
//       //end of playback check
//       if(x.size() == 0) playBackTimer->stop();
//       //
//       customplot->replot();
//   }



ui->widget->replot();
}

void MainWindow::on_ClearButton_clicked()
{
    //отчищает полотно
     ui->widget->clearPlottables();
     ui->widget->replot();
      allA.clear();
}

void MainWindow::on_checkBox_clicked()
{
    if (ui->checkBox->isChecked()){

        ui->widget->xAxis->setRange(-10.8, 10.8);
        ui->widget->yAxis->setRange(-10.8, 10.8);
    }else{
       if(A>range){
           range = A * 1.2;
           ui->widget->xAxis->setRange(range*(-1), range);
           ui->widget->yAxis->setRange(range*(-1), range);
       }

       else{
              double *r = std::max_element(std::begin(allA), std::end(allA));
              double k = *r;
              ui->widget->xAxis->setRange(k *(-1.2), k*1.2);
              ui->widget->yAxis->setRange(k *(-1.2), k*1.2);
          }
    }

    ui->widget->replot();

}

void MainWindow::on_DefaultButton_clicked()
{
    ui->RCurveSlider->setValue(190);
    ui->GCurveSlider->setValue(85);
    ui->BCurveSlider->setValue(85);
    ui->AFactor->setValue(1);
    ui->BFactor->setValue(2);
    ui->CFactor->setValue(7);
    ui->SizeLineSlider->setValue(2);
}
