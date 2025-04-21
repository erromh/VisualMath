#include "mainwindow.h"
#include "graphwidget.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  resize(1200, 800);
  QWidget *central = new QWidget(this);
  setCentralWidget(central);

  mainLayout = new QHBoxLayout(central);

  leftPanel = new QVBoxLayout();
  QLabel *label = new QLabel("Введите точки (формат: x1,y1; x2,y2; ...):");
  inputField = new QLineEdit();
  QPushButton *plotButton = new QPushButton("Построить график");

  leftPanel->addWidget(label);
  leftPanel->addWidget(inputField);
  leftPanel->addWidget(plotButton);
  leftPanel->addStretch();

  mainLayout->addLayout(leftPanel, 1);

  graphWidget = new GraphWidget();
  mainLayout->addWidget(graphWidget, 3);

  connect(plotButton, &QPushButton::clicked, this, &MainWindow::onPlotClicked);

  setupAdditionalPanel();
}

void MainWindow::onPlotClicked() {
  QVector<QPointF> points;

  const QString inputText = inputField->text();
  const QStringList pairs = inputText.split(';', Qt::SkipEmptyParts);

  for (const QString &pair : pairs) {
    const QStringList coords = pair.trimmed().split(',');

    if (coords.size() == 2) {
      bool okX = false, okY = false;
      double x = coords[0].toDouble(&okX);
      double y = coords[1].toDouble(&okY);

      if (okX && okY)
        points.append(QPointF(x, y));
    }
  }

  graphWidget->setFunctionData(points);
}

void MainWindow::setupAdditionalPanel() {
  additionalPanel = new QWidget(this);
  additionalPanelLayout = new QVBoxLayout(additionalPanel);

  auto extraGroup = new QGroupBox("Дополнительно");
  auto extraLayout = new QVBoxLayout();

  distanceButton = new QPushButton("Определить расстояние");

  auto pointInputsLayout = new QHBoxLayout();

  point1Edit = new QLineEdit();
  point1Edit->setReadOnly(true);

  point2Edit = new QLineEdit();
  point2Edit->setReadOnly(true);

  pointInputsLayout->addWidget(point1Edit);
  pointInputsLayout->addWidget(point2Edit);

  // Поле для вывода результата
  resultOutput = new QLineEdit();
  resultOutput->setReadOnly(true);
  resultOutput->setPlaceholderText("Здесь будет результат");

  extraLayout->addWidget(distanceButton);
  extraLayout->addLayout(pointInputsLayout);
  extraLayout->addWidget(resultOutput);
  extraGroup->setLayout(extraLayout);
  extraGroup->setVisible(true);

  leftPanel->addWidget(extraGroup);

  connect(distanceButton, &QPushButton::clicked, this, [=]() {
    graphWidget->startDistanceMeasurement(
        [=](const QString &resultText) { resultOutput->setText(resultText); });
  });

  connect(graphWidget, &GraphWidget::point1Selected, this,
          [=](double x, double y) {
            point1Edit->setText(QString("X: %1, Y: %2").arg(x).arg(y));
          });

  connect(graphWidget, &GraphWidget::point2Selected, this,
          [=](double x, double y) {
            point2Edit->setText(QString("X: %1, Y: %2").arg(x).arg(y));
          });

  // === Арифметические преобразования ===
  auto arithmeticGroup = new QGroupBox("Арифметические преобразования");
  auto arithmeticLayout = new QHBoxLayout(arithmeticGroup);

  shiftLabel = new QLabel("Сдвиг по оси Y:");
  shiftSpinBox = new QDoubleSpinBox();
  shiftSpinBox->setRange(-1000.0, 1000.0);
  shiftSpinBox->setSingleStep(0.1);

  applyShiftButton = new QPushButton("Применить");

  arithmeticLayout->addWidget(shiftLabel);
  arithmeticLayout->addWidget(shiftSpinBox);
  arithmeticLayout->addWidget(applyShiftButton);

  additionalPanelLayout->addWidget(arithmeticGroup);

  //  connect(applyShiftButton, &QPushButton::clicked, this, [=]() {
  //    double offset = shiftSpinBox->value();
  //    graphWidget->applyShift(offset);
  //  });

  // mainLayout->addWidget(additionalPanel);

  leftPanel->addWidget(additionalPanel);
}
