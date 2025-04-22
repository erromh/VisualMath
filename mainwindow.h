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

  QWidget *additionalPanel;
  QVBoxLayout *additionalPanelLayout;

  QPushButton *distanceButton;
  QTextEdit *distanceResult;

  QLabel *shiftLabel;
  QDoubleSpinBox *shiftSpinBox;
  QPushButton *applyShiftButton;
  QComboBox *operationComboBox;
  QLineEdit *operationValueEdit;

  QPushButton *undoButton;
};
