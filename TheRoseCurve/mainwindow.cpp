#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow (QWidget *parent) :
    QMainWindow (parent),
    ui (new Ui::MainWindow) {
    ui->setupUi (this);
    ui->widget->hide();//при запуске формы область построения кривой спрятана
    setupDefaultValues();//при запуске формы устанавливаем значения по умолчанию
}

void MainWindow::setupDefaultValues() {//значения по умолчанию
    //Цвет кривой, задающийся в rgb
    ui->RCurveSlider->setValue (190);
    ui->GCurveSlider->setValue (85);
    ui->BCurveSlider->setValue (85);
    //Цвет трекера
    ui->RTrackerSlider->setValue (54);
    ui->GTrackerSlider->setValue (160);
    ui->BTrackerSlider->setValue (209);
    //Значения коэффициентов в уравнении
    ui->AFactor->setValue (3);
    ui->AFactor->setMaximum (20);
    ui->BFactor->setValue (2);
    ui->BFactor->setMaximum (20);
    ui->CFactor->setValue (5);
    ui->CFactor->setMaximum (20);
    //Толщина линии
    ui->SizeLineSlider->setValue (4);
    //Размер трекера
    ui->SizeTracerSlider->setValue (10);
}

void MainWindow::on_BuildButton_clicked() {//при нажатии на кнопку "Построить"
    ui->widget->show();//показать пространство для построения кривой
    //область построения кривой отчищается при каждом нажатии
    areaCleaner();
    //снимаем значения коэффициентов
    A = ui->AFactor->value();
    B = ui->BFactor->value();
    C = ui->CFactor->value();
    //вызываем функцию построения кривой
    setupRoseCurve (ui->widget);
    //вызываем функцию построения сетки
    forGridView (ui->widget);
    //обновление поверхности области построения кривой (widget)
    ui->widget->replot();
}

void MainWindow::forGridView (QCustomPlot
                              *widget) {//внешний вид и координаты сетки
    //настраиваем внешний вид пера для отрисовки линий сетки
    QPen penForGrid (QColor (80, 80, 80, 200), //цвет в rbg + альфа-канал
                     1, //толщина линии
                     Qt::DotLine); //тип линии(пунктирная)
    const int POINTS_FOR_GRID = 360; //количество точек сетки
    //шаг основных делений на оси
    QSharedPointer<QCPAxisTickerFixed> fixedTicker (new QCPAxisTickerFixed);
    widget->xAxis->setTicker (fixedTicker);
    widget->yAxis->setTicker (fixedTicker);
    fixedTicker->setTickStep (1.0); // шаг основных делений осей
    fixedTicker->setScaleStrategy (QCPAxisTickerFixed::ssNone);
    auto radiusOfPetals = A; //радиус наибольшего круга сетки равен
    //задающему длину лепестков
    auto range = radiusOfPetals * 1.2;
    //устанавливаем интервалы области построения осям
    widget->xAxis->setRange (range * (-1), range);
    widget->yAxis->setRange (range * (-1), range);

    //рисуем кривые для линий сетки
    for (int j = 0; j <= radiusOfPetals; j++) {
        //создаем вектор для точек круга
        QVector <QCPCurveData> temp (POINTS_FOR_GRID);

        for (int i = 0; i < POINTS_FOR_GRID; i++) {
            //переводим угол в радианы
            double theta = (i / 180.0) * M_PI;
            //уравнения для построения окружностей в полярных координатах
            temp[i] = QCPCurveData (i, j * qCos (1.0 * theta), j * qSin (1.0 * theta));
        }

        pointsOfGrid.insert (j, temp);//добавляем ключ и значение
        //(радиус и соответствующие ему координаты окружности)
    }

    //задаем новый размер
    rounds.resize (radiusOfPetals + 1);

    for (int i = 0; i <= radiusOfPetals; i++) {
        //создаем новую кривую
        rounds[i] = new QCPCurve (widget->xAxis, widget->yAxis);
        //добавляем данные
        rounds[i]->data()->set (pointsOfGrid[i]);
        //устанавливаем перо
        rounds[i]->setPen (penForGrid);
    }

    //задаем смещение осей, чтобы они проходили через центр области построения
    widget->xAxis->setOffset (-185);
    widget->yAxis->setOffset (-185);
    //задаем отступ от краев формы до границ области графика
    widget->xAxis->setPadding (0);
    widget->yAxis->setPadding (0);
    //не показывать на осях промежуточные деления
    widget->xAxis->setSubTicks (0);
    widget->yAxis->setSubTicks (0);
    //убрать подписи делений по оси у
    widget->yAxis->setTickLabels (0);
    widget->xAxis->setTickLabels (0);
    //убрать линии сетки
    widget->yAxis->grid()->setVisible (0);
    widget->xAxis->grid()->setVisible (0);
    //внешний вид линий осей
    widget->xAxis->setBasePen (penForGrid);
    widget->yAxis->setBasePen (penForGrid);
}

void MainWindow::setupRoseCurve (QCustomPlot *widget) {
    //создаем кривую-подложку
    QCPCurve *newCurve = new QCPCurve (widget->xAxis, widget->yAxis);
    //количество точек построения
    pointCount = setPointCount();
    //вектор координат
    QVector <QCPCurveData> temp (pointCount);

    for (int i = 0; i < pointCount; i++) {

        double theta = (i / 180.0) *
                       M_PI; //переводим градусы в радианы
        //формула кривой в переводе на декартову систему из полярных координат
        temp[i] = QCPCurveData (i,
                                A * qSin (((B * 1.0) / (1.0 * C)) * theta) * qCos (1 * theta),
                                A * qSin (((B * 1.0) / (1.0 * C)) * theta) * qSin (1 * theta));
    }

    //добавляем координаты для отрисовки кривой
    newCurve->data()->set (temp);
    //Цвет кривой
    int RLine = ui->RCurveSlider->value();
    int GLine = ui->GCurveSlider->value();
    int BLine = ui->BCurveSlider->value();
    //Значение толщины ЛИНИИ
    int LineWidth = ui->SizeLineSlider->value();
    //настройка пера для анимированной кривой
    QPen penForCurve (QColor (RLine, GLine, BLine, 255),
                      LineWidth,
                      Qt::SolidLine,
                      Qt::SquareCap,
                      Qt::MiterJoin);
    //настройка пера для кривой-подложки
    QPen penForDrawCurve (QColor (200, 200, 200, 100),
                          LineWidth,
                          Qt::SolidLine,
                          Qt::SquareCap,
                          Qt::MiterJoin);
    //создаем анимированную кривую
    curveByPoints = new QCPCurve (widget->xAxis, widget->yAxis);
    curveByPoints->setPen (penForCurve);
    curveByPoints->setAntialiased (1);
    newCurve->setPen (penForDrawCurve);
    newCurve->setAntialiased (1);//включаем сглаживание
    //создаем трекер
    QCPItemTracer *tracer = new QCPItemTracer (ui->widget);
    curveTracer = tracer;
    //настраиваем внешний вид трекера
    tracer->setStyle (QCPItemTracer::tsSquare);
    //цвет, толщина и тип границы трекера
    QPen penForTracer (QColor (255, 255, 255), //белый
                       1,
                       Qt::SolidLine,
                       Qt::SquareCap,
                       Qt::MiterJoin);
    //устанавливаем перо для трекера
    tracer->setPen (penForTracer);
    //цвет трекера(заливка)
    int RTracer = ui->RTrackerSlider->value();
    int GTracer = ui->GTrackerSlider->value();
    int BTracer = ui->BTrackerSlider->value();
    tracer->setBrush (QColor (RTracer, GTracer, BTracer));
    tracer->setAntialiased (0);//отключаем сглаживание
    //берем значение размера трекера
    int SizeTracer = ui->SizeTracerSlider->value();
    tracer->setSize (
            SizeTracer);//устанавливаем значение трекера
    timeElapsed = 0;//переменная для отсчета времени
    connect (timer, SIGNAL (timeout()), this, SLOT (slotTimer()));
    timer->start (0); // запускаем таймер
}

int gcd (int a, int b) {
	return (b == 0) ? abs(a) : gcd(b, a % b);
}

void reduce (int &a, int &b) {

    int c = gcd (a, b);
    a /= c;
    b /= c;
}

int MainWindow::setPointCount() {

    const int halfTurn = 180;
    int petalsAmount = 0;
    reduce (B, C);

    if (C == 1 && B % 2 == 0) {

        petalsAmount = 2 * B;

    } else if (C == 1 && B % 2 != 0) {

        petalsAmount = B;

    } else if ((B % 2 != 0) && ( C % 2 != 0) ) {

        petalsAmount = B;

    } else {petalsAmount = 2 * B;}

    return ((halfTurn * C * petalsAmount) / B) + 1;
}

void MainWindow::slotTimer() {
    key = timeElapsed;// time
    double x, y;

    if (key < pointCount) { // add point

        double theta = (key / 180.0) * M_PI;
        x/*[key]*/ = (A * qSin (((B * 1.0) / (1.0 * C)) * theta)
                      * qCos (1 * theta));
        y/*[key]*/ = (A * qSin (((B * 1.0) / (1.0 * C)) * theta)
                      * qSin (1 * theta));
        x2.push_back (x);
        y2.push_back (y);
        curveTracer->position->setCoords (x/*[key]*/, y/*[key]*/);
    }

    curveByPoints->setData (x2, y2);
    timeElapsed += 2;
    ui->widget->replot();
}

void MainWindow::areaCleaner() {
    //отчищает полотно
    ui->widget->clearPlottables();//стирает предыдущий график и сетку
    ui->widget->clearItems();//стирает трекер
    pointsOfGrid.clear();
    QMap<int, QVector<QCPCurveData>> (pointsOfGrid).swap (pointsOfGrid);
    //отчистка векторов
    rounds.clear();
    rounds.squeeze();
    x2.clear();
    x2.squeeze();
    y2.clear();
    y2.squeeze();
    //останавливаем таймер
    timeElapsed = 0;
    timer->stop();
    ui->widget->replot();//обновление области построения кривой
}

void MainWindow::on_DefaultButton_clicked() {
    setupDefaultValues();//устанавливает значения по умолчанию
}

MainWindow::~MainWindow() {
    delete ui;
}
