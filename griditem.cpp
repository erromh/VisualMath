#include "griditem.h"
#include <QGraphicsView>
#include <QPainter>
#include <QWidget>
#include <cmath>

QRectF GridItem::boundingRect() const {
  return scene() ? scene()->sceneRect() : QRectF(-400, -400, 800, 600);
}

void GridItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *,
                     QWidget *) {
  if (!scene())
    return;

  QRectF rect = scene()->sceneRect();

  constexpr int spacing = 10;

  QPen gridPen(Qt::lightGray, 0);
  painter->setPen(gridPen);

  // Вертикальные линии
  for (qreal x = std::floor(rect.left() / spacing) * spacing; x <= rect.right();
       x += spacing)
    painter->drawLine(QPointF(x, rect.top()), QPointF(x, rect.bottom()));

  // Горизонтальные линии
  for (qreal y = std::floor(rect.top() / spacing) * spacing; y <= rect.bottom();
       y += spacing)
    painter->drawLine(QPointF(rect.left(), y), QPointF(rect.right(), y));

  QPen axisPen(Qt::black, 0.5);
  painter->setPen(axisPen);
  painter->drawLine(QPointF(0, rect.top()), QPointF(0, rect.bottom()));
  painter->drawLine(QPointF(rect.left(), 0), QPointF(rect.right(), 0));

  painter->save();

  QTransform noScale;
  noScale.scale(1.0, -1.0);
  painter->setWorldTransform(noScale * painter->worldTransform());

  QFont font = painter->font();
  font.setPointSize(2);
  painter->setFont(font);
  painter->setPen(Qt::darkGray);

  for (qreal x = std::floor(rect.left() / spacing) * spacing; x <= rect.right();
       x += spacing) {
    if (qFuzzyCompare(x, 0.0))
      continue;
    painter->drawText(QPointF(x + 2, -2), QString::number(x));
  }

  for (qreal y = std::floor(rect.top() / spacing) * spacing; y <= rect.bottom();
       y += spacing) {
    if (qFuzzyCompare(y, 0.0))
      continue;
    painter->drawText(QPointF(2, -y - 2), QString::number(y));
  }
  painter->restore();
}

void GridItem::setStep(qreal step) {
  m_step = step;
  update();
}

void GridItem::setSize(qreal width, qreal height) {
  m_width = width;
  m_height = height;
  update();
}
