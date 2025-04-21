#include "functionmodel.h"

void FunctionModel::setData(const QVector<double> &data) { dataPoints = data; }

const QVector<double> &FunctionModel::getData() const { return dataPoints; }
