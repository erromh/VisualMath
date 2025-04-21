#pragma once
#include "graphwidget.h"
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);

private slots:
  void onPlotClicked();
  void setupAdditionalPanel();
  void applySelectedOperation();
  void resetAdditionalFields();

private:
  QHBoxLayout *mainLayout;
  QVBoxLayout *leftPanel;
  QLineEdit *inputField;
  GraphWidget *graphWidget;

  QLineEdit *point1Edit;
  QLineEdit *point2Edit;
  QLineEdit *resultOutput;

  QGroupBox *extraGroup;

  QWidget *additionalPanel = nullptr;
  QVBoxLayout *additionalPanelLayout = nullptr;

  // Расстояние
  QPushButton *distanceButton = nullptr;
  QTextEdit *distanceResult = nullptr;

  // Арифметика
  QLabel *shiftLabel = nullptr;
  QDoubleSpinBox *shiftSpinBox = nullptr;
  QPushButton *applyShiftButton = nullptr;
  QComboBox *operationComboBox = nullptr;
  QLineEdit *operationValueEdit = nullptr;

  QPushButton *undoButton = nullptr;
};
