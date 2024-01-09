#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "animatedarrow.h"
#include <QLayout>
#include <QGroupBox>
#include <QComboBox>
#include <QSpinBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QPointF start(100, 400);
    QPointF end(200, 500);
    AnimatedArrow* animatedArrow1 = new AnimatedArrow(this, start, end, true, this);
    QObject::connect(this, &MainWindow::parentResized, animatedArrow1, &AnimatedArrow::parentResized);
    QObject::connect(this, &MainWindow::coordinatesChanged, animatedArrow1, &AnimatedArrow::coordinatesChanged);
    QObject::connect(this, &MainWindow::setPen, animatedArrow1, &AnimatedArrow::setPen);

    QWidget* mainWidget = new QWidget(this);
    setCentralWidget(mainWidget);
    QVBoxLayout* mainLayout = new QVBoxLayout(mainWidget);

    QWidget* parameterWidget = new QWidget(mainWidget);
    mainLayout->addWidget(parameterWidget);
    QWidget* arrowWidget = new QWidget(mainWidget);
    mainLayout->addWidget(arrowWidget);

    QHBoxLayout* parameterLayout = new QHBoxLayout(parameterWidget);
    parameterWidget->setLayout(parameterLayout);

    QGroupBox* animationGroupBox = new QGroupBox(parameterWidget);
    animationGroupBox->setTitle("Animation:");
    parameterLayout->addWidget(animationGroupBox);
    QGridLayout* animationLayout = new QGridLayout(animationGroupBox);

    QComboBox* typeOfLineComboBox = new QComboBox(animationGroupBox);
    typeOfLineComboBox->addItems({"Straight line"/*, "Dotted line", "Curved line"*/});
    typeOfLineComboBox->setCurrentIndex(0);
    QObject::connect(typeOfLineComboBox, &QComboBox::currentIndexChanged, animatedArrow1, qOverload<int>(&AnimatedArrow::setTypeOfLine));
    animationLayout->addWidget(typeOfLineComboBox, 0, 0, 1, 2);

    QComboBox* typeOfArrowheadComboBox = new QComboBox(animationGroupBox);
    typeOfArrowheadComboBox->addItems({"No arrowhead", "Arrow", "Big Dot"/*, "Pointy"*/});
    typeOfArrowheadComboBox->setCurrentIndex(1);
    QObject::connect(typeOfArrowheadComboBox, &QComboBox::currentIndexChanged, animatedArrow1, qOverload<int>(&AnimatedArrow::setTypeOfArrowhead));
    animationLayout->addWidget(typeOfArrowheadComboBox, 1, 0, 1, 2);

    QComboBox* typeOfAnimationComboBox = new QComboBox(animationGroupBox);
    typeOfAnimationComboBox->addItems({"No animation", "Grow", "Fade in", "Fade out", "Fade in and out"/*, "Fade from start to end"*/});
    typeOfAnimationComboBox->setCurrentIndex(0);
    QObject::connect(typeOfAnimationComboBox, &QComboBox::currentIndexChanged, animatedArrow1, qOverload<int>(&AnimatedArrow::setTypeOfAnimation));
    animationLayout->addWidget(typeOfAnimationComboBox, 2, 0, 1, 2);

    QComboBox* typeOfRepetitionComboBox = new QComboBox(animationGroupBox);
    typeOfRepetitionComboBox->addItems({"No animation", "Animate once", "Continuing Animation"});
    typeOfRepetitionComboBox->setCurrentIndex(0);
    QObject::connect(typeOfRepetitionComboBox, &QComboBox::currentIndexChanged, animatedArrow1, qOverload<int>(&AnimatedArrow::setTypeOfRepetition));
    animationLayout->addWidget(typeOfRepetitionComboBox, 3, 0, 1, 2);

    QLabel* durationLabel = new QLabel("Duration (msecs):", animationGroupBox);
    animationLayout->addWidget(durationLabel, 4, 0);

    QSpinBox* durationSpinBox = new QSpinBox(animationGroupBox);
    durationSpinBox->setRange(1000, 10000);
    durationSpinBox->setValue(1000);
    animationLayout->addWidget(durationSpinBox, 4, 1);
    QObject::connect(durationSpinBox, &QSpinBox::valueChanged, animatedArrow1, &AnimatedArrow::setDuration);

    QGroupBox* positionGroupBox = new QGroupBox(parameterWidget);
    positionGroupBox->setTitle("Position:");
    parameterLayout->addWidget(positionGroupBox);
    QGridLayout* positionLayout = new QGridLayout(positionGroupBox);

    QLabel* startLabel = new QLabel("Start:", positionGroupBox);
    positionLayout->addWidget(startLabel, 0, 0);

    m_startXSpinBox = new QSpinBox(positionGroupBox);
    m_startXSpinBox->setRange(0, 800);
    m_startXSpinBox->setValue(start.x());
    positionLayout->addWidget(m_startXSpinBox, 0, 1);
    QObject::connect(m_startXSpinBox, &QSpinBox::valueChanged, this, &MainWindow::startEndChanged);

    m_startYSpinBox = new QSpinBox(positionGroupBox);
    m_startYSpinBox->setRange(300, 600);
    m_startYSpinBox->setValue(start.y());
    positionLayout->addWidget(m_startYSpinBox, 0, 2);
    QObject::connect(m_startYSpinBox, &QSpinBox::valueChanged, this, &MainWindow::startEndChanged);

    QLabel* endLabel = new QLabel("End:", positionGroupBox);
    positionLayout->addWidget(endLabel, 1, 0);

    m_endXSpinBox = new QSpinBox(positionGroupBox);
    m_endXSpinBox->setRange(0, 800);
    m_endXSpinBox->setValue(end.x());
    positionLayout->addWidget(m_endXSpinBox, 1, 1);
    QObject::connect(m_endXSpinBox, &QSpinBox::valueChanged, this, &MainWindow::startEndChanged);

    m_endYSpinBox = new QSpinBox(positionGroupBox);
    m_endYSpinBox->setRange(300, 600);
    m_endYSpinBox->setValue(end.y());
    positionLayout->addWidget(m_endYSpinBox, 1, 2);
    QObject::connect(m_endYSpinBox, &QSpinBox::valueChanged, this, &MainWindow::startEndChanged);

    QGroupBox* penGroupBox = new QGroupBox(parameterWidget);
    penGroupBox->setTitle("Pen:");
    parameterLayout->addWidget(penGroupBox);
    QGridLayout* penLayout = new QGridLayout(penGroupBox);

    QLabel* widthLabel = new QLabel("Width:", penGroupBox);
    penLayout->addWidget(widthLabel, 0, 0);

    m_widthSpinBox = new QSpinBox(penGroupBox);
    m_widthSpinBox->setRange(1, 10);
    m_widthSpinBox->setValue(1);
    penLayout->addWidget(m_widthSpinBox, 0, 1, 1, 3);
    QObject::connect(m_widthSpinBox, &QSpinBox::valueChanged, this, &MainWindow::penChanged);

    QLabel* joinLabel = new QLabel("Join:", penGroupBox);
    penLayout->addWidget(joinLabel, 1, 0);

    m_joinComboBox = new QComboBox(penGroupBox);
    m_joinComboBox->addItems({"Bevel Join", "Miter Join", "Round Join"});
    m_joinComboBox->setCurrentIndex(0);
    penLayout->addWidget(m_joinComboBox, 1, 1, 1, 3);
    QObject::connect(m_joinComboBox, &QComboBox::currentIndexChanged, this, &MainWindow::penChanged);

    QLabel* colorLabel = new QLabel("Color (RGB):", penGroupBox);
    penLayout->addWidget(colorLabel, 2, 0);

    m_colorRSpinBox = new QSpinBox(penGroupBox);
    m_colorRSpinBox->setRange(0, 255);
    m_colorRSpinBox->setValue(0);
    penLayout->addWidget(m_colorRSpinBox, 2, 1);
    QObject::connect(m_colorRSpinBox, &QSpinBox::valueChanged, this, &MainWindow::penChanged);

    m_colorGSpinBox = new QSpinBox(penGroupBox);
    m_colorGSpinBox->setRange(0, 255);
    m_colorGSpinBox->setValue(0);
    penLayout->addWidget(m_colorGSpinBox, 2, 2);
    QObject::connect(m_colorGSpinBox, &QSpinBox::valueChanged, this, &MainWindow::penChanged);

    m_colorBSpinBox = new QSpinBox(penGroupBox);
    m_colorBSpinBox->setRange(0, 255);
    m_colorBSpinBox->setValue(0);
    penLayout->addWidget(m_colorBSpinBox, 2, 3);
    QObject::connect(m_colorBSpinBox, &QSpinBox::valueChanged, this, &MainWindow::penChanged);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::startEndChanged()
{
    emit coordinatesChanged(QPointF(m_startXSpinBox->value(), m_startYSpinBox->value()), QPointF(m_endXSpinBox->value(), m_endYSpinBox->value()));
}

void MainWindow::penChanged()
{
    QPen pen;
    pen.setWidth(m_widthSpinBox->value());

    switch (m_joinComboBox->currentIndex()) {
    case 0:
        pen.setJoinStyle(Qt::BevelJoin);
        break;
    case 1:
        pen.setJoinStyle(Qt::MiterJoin);
        break;
    case 2:
        pen.setJoinStyle(Qt::RoundJoin);
        break;
    default:
        break;
    }

    QColor color(m_colorRSpinBox->value(), m_colorGSpinBox->value(), m_colorBSpinBox->value());
    pen.setColor(color);
    emit setPen(pen);
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    emit parentResized(event->size());
}

