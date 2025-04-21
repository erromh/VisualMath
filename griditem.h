#pragma once
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPainter>
#include <QPen>
#include <QWidget>

class GridItem : public QGraphicsItem {
public:
  QRectF boundingRect() const override;
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
             QWidget *widget) override;
  void setStep(qreal step);
  void setSize(qreal width, qreal height);

private:
  qreal m_width;
  qreal m_height;
  qreal m_step;
};
