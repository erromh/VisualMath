#include "mainwindow.h"
#include "graphwidget.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  resize(1600, 950);
  QWidget *central = new QWidget(this);
  if (!central)
    return;
  setCentralWidget(central);

  mainLayout = new QHBoxLayout(central);
  if (!mainLayout)
    return;

  auto doubleValidator = new QDoubleValidator(this);
  if (!doubleValidator)
    return;
  doubleValidator->setNotation(QDoubleValidator::StandardNotation);

  leftPanel = new QVBoxLayout();
  if (!leftPanel)
    return;

  QLabel *label = new QLabel("Введите точки (формат: x1,y1; x2,y2; ...):");
  inputField = new QLineEdit();
  QPushButton *plotButton = new QPushButton("Построить график");

  if (!label || !inputField || !plotButton)
    return;

  leftPanel->addWidget(label);
  leftPanel->addWidget(inputField);
  leftPanel->addWidget(plotButton);

  mainLayout->addLayout(leftPanel, 1);

  graphWidget = new GraphWidget();
  if (!graphWidget)
    return;

  mainLayout->addWidget(graphWidget, 3);

  connect(plotButton, &QPushButton::clicked, this, &MainWindow::onPlotClicked);

  setupAdditionalPanel();

  leftPanel->addStretch();
}

void MainWindow::onPlotClicked() {
  if (!inputField || !graphWidget)
    return;
  resetAdditionalFields();
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
  auto extraGroup = new QGroupBox("Дополнительно");
  if (!extraGroup)
    return;
  auto extraLayout = new QVBoxLayout(extraGroup);
  if (!extraLayout)
    return;

  distanceButton = new QPushButton("Определить расстояние");

  point1Edit = new QLineEdit();
  point2Edit = new QLineEdit();
  if (!point1Edit || !point2Edit)
    return;

  point1Edit->setReadOnly(true);
  point2Edit->setReadOnly(true);

  auto pointInputsLayout = new QHBoxLayout();
  pointInputsLayout->addWidget(point1Edit);
  pointInputsLayout->addWidget(point2Edit);

  resultOutput = new QLineEdit();
  if (!resultOutput)
    return;

  resultOutput->setReadOnly(true);
  resultOutput->setPlaceholderText("Здесь будет результат");

  extraLayout->addWidget(distanceButton);
  extraLayout->addLayout(pointInputsLayout);
  extraLayout->addWidget(resultOutput);

  auto arithmeticGroup = new QGroupBox("Арифметические преобразования");
  auto arithmeticLayout = new QHBoxLayout(arithmeticGroup);
  if (!arithmeticLayout)
    return;

  operationComboBox = new QComboBox();
  if (!operationComboBox)
    return;

  operationComboBox->addItem("Выберите операцию");
  operationComboBox->addItem("Сдвинуть вверх");
  operationComboBox->addItem("Сдвинуть вниз");
  operationComboBox->addItem("Сдвинуть влево");
  operationComboBox->addItem("Сдвинуть вправо");
  operationComboBox->addItem("Растянуть по Y");
  operationComboBox->addItem("Сжать по Y");
  operationComboBox->addItem("Растянуть по X");
  operationComboBox->addItem("Сжать по X");
  operationComboBox->addItem("Отразить по оси X");
  operationComboBox->addItem("Отразить по оси Y");
  operationComboBox->addItem("Инвертировать X и Y");

  auto doubleValidator = new QDoubleValidator(this);
  if (!doubleValidator)
    return;
  doubleValidator->setNotation(QDoubleValidator::StandardNotation);

  operationValueEdit = new QLineEdit();
  if (!operationValueEdit)
    return;

  operationValueEdit->setPlaceholderText("Значение (опц.)");
  operationValueEdit->setValidator(doubleValidator);

  auto applyOperationButton = new QPushButton("Применить операцию");
  if (!applyOperationButton)
    return;

  arithmeticLayout->addWidget(operationComboBox);
  arithmeticLayout->addWidget(operationValueEdit);
  arithmeticLayout->addWidget(applyOperationButton);

  auto additionalLayout = new QVBoxLayout();
  if (!additionalLayout)
    return;

  additionalLayout->addWidget(extraGroup);
  additionalLayout->addWidget(arithmeticGroup);

  undoButton = new QPushButton("Отменить последнее действие");
  if (!undoButton)
    return;

  additionalLayout->addWidget(undoButton);

  additionalPanel = new QWidget(this);
  if (!additionalPanel)
    return;
  additionalPanel->setLayout(additionalLayout);

  leftPanel->addWidget(additionalPanel);

  if (graphWidget) {
    connect(distanceButton, &QPushButton::clicked, this, [=]() {
      resetAdditionalFields();
      graphWidget->startDistanceMeasurement([=](const QString &resultText) {
        resultOutput->setText(resultText);
      });
    });

    connect(graphWidget, &GraphWidget::point1Selected, this,
            [=](double x, double y) {
              point1Edit->setText(QString("X: %1, Y: %2").arg(x).arg(y));
            });

    connect(graphWidget, &GraphWidget::point2Selected, this,
            [=](double x, double y) {
              point2Edit->setText(QString("X: %1, Y: %2").arg(x).arg(y));
            });

    connect(applyOperationButton, &QPushButton::clicked, this,
            &MainWindow::applySelectedOperation);

    connect(undoButton, &QPushButton::clicked, this,
            [=]() { graphWidget->undoLastAction(); });
  }
}

void MainWindow::applySelectedOperation() {
  resetAdditionalFields();

  if (!graphWidget || !operationComboBox)
    return;

  QString selectedOp = operationComboBox->currentText();
  QString inputText = operationValueEdit->text();

  bool ok = false;
  double value = inputText.toDouble(&ok);

  if (!ok) {
    if (selectedOp.contains("Сдвинуть"))
      value = 10.0;
    else if (selectedOp.contains("Растянуть"))
      value = 1.5;
    else if (selectedOp.contains("Сжать"))
      value = 0.75;
    else
      value = 0.0;
  }

  graphWidget->applyOperation(selectedOp, value);
}

void MainWindow::resetAdditionalFields() {
  if (point1Edit)
    point1Edit->clear();
  if (point2Edit)
    point2Edit->clear();
  if (resultOutput)
    resultOutput->clear();
}
