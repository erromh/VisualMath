#pragma once
#include "griditem.h"
#include <QGraphicsSimpleTextItem>
#include <QGraphicsView>
#include <QKeyEvent>
#include <functional>
#include <stack>

class GraphWidget : public QGraphicsView {
  Q_OBJECT
public:
  GraphWidget(QWidget *parent = nullptr);
  void setFunctionData(const QVector<QPointF> &data);
  void startDistanceMeasurement(std::function<void(const QString &)> callback);
  void applyOperation(const QString &operation, qreal &value);

  void saveStateForUndo();
  void undoLastAction();

protected:
  void wheelEvent(QWheelEvent *event) override;
  void keyPressEvent(QKeyEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;
  void mousePressEvent(QMouseEvent *event) override;

private:
  GridItem *gridItem = nullptr;
  QGraphicsSimpleTextItem *hoverLabel = nullptr;

  QVector<QPointF> functionData;
  qreal currentScale = 1.0;
  const qreal scaleFactor = 1.2;
  const qreal minScale = 0.1;
  const qreal maxScale = 10.0;

  bool isMeasuringDistance = false;
  QPointF firstPoint;
  bool firstPointSelected = false;
  std::function<void(const QString &)> onDistanceComputed;

  std::stack<QVector<QPointF>> undoStack;
  QVector<QPointF> currentGraphPoints;
  QGraphicsPathItem *functionGraphItem = nullptr;

private:
  void updateGraph();

signals:
  void point1Selected(double x, double y);
  void point2Selected(double x, double y);
};
