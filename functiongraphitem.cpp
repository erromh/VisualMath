#include "functiongraphitem.h"
#include <QGraphicsScene>
#include <QPainter>

FunctionGraphItem::FunctionGraphItem(const QVector<QPointF> &dataPoints,
                                     qreal scaleFactor)
    : points(dataPoints), _scale(scaleFactor) {}

QRectF FunctionGraphItem::boundingRect() const {
  return QRectF(-10000, -10000, 20000, 20000);
}

void FunctionGraphItem::paint(QPainter *painter,
                              const QStyleOptionGraphicsItem *, QWidget *) {
  if (points.isEmpty())
    return;

  painter->setRenderHint(QPainter::Antialiasing);
  QPen pen(Qt::red, 1);
  painter->setPen(pen);

  QPainterPath path;
  QPointF first = points.first() * _scale;
  path.moveTo(first);

  for (const QPointF &p : points)
    path.lineTo(p * _scale);

  painter->drawPath(path);
}
