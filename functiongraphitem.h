#pragma once
#include <QGraphicsItem>
#include <QPointF>
#include <QVector>

class FunctionGraphItem : public QGraphicsItem {
public:
  explicit FunctionGraphItem(const QVector<QPointF> &dataPoints,
                             qreal scaleFactor);
  QRectF boundingRect() const override;
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *,
             QWidget *) override;

private:
  QVector<QPointF> points;
  qreal _scale;
};
