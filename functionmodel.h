#ifndef FUNCTIONMODEL_H
#define FUNCTIONMODEL_H

#include <QVector>

class FunctionModel {
public:
  void setData(const QVector<double> &data);
  const QVector<double> &getData() const;

private:
  QVector<double> dataPoints;
};

#endif // FUNCTIONMODEL_H
