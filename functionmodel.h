#pragma once
#include <QVector>

class FunctionModel {
public:
  void setData(const QVector<double> &data);
  const QVector<double> &getData() const;

private:
  QVector<double> dataPoints;
};
