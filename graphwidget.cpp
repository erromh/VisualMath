#include "graphwidget.h"
#include "functiongraphitem.h"
#include <QMouseEvent>
#include <QToolTip>
#include <QWheelEvent>
#include <QtMath>

GraphWidget::GraphWidget(QWidget *parent) : QGraphicsView(parent) {
  auto scene = new QGraphicsScene(this);
  scene->setSceneRect(-400, -300, 800, 600);

  setScene(scene);
  setRenderHint(QPainter::Antialiasing);
  setDragMode(QGraphicsView::ScrollHandDrag);
  setMouseTracking(true);

  scene->addItem(new GridItem());

  hoverLabel = new QGraphicsSimpleTextItem();
  hoverLabel->setBrush(Qt::red);
  hoverLabel->setZValue(100);
  scene->addItem(hoverLabel);

  scale(5, -5);
}

void GraphWidget::setFunctionData(const QVector<QPointF> &data) {
  scene()->clear();
  constexpr qreal gridScale = 1.0;
  scene()->addItem(new GridItem());
  scene()->addItem(new FunctionGraphItem(data, gridScale));

  functionData = data;
  hoverLabel = new QGraphicsSimpleTextItem();
  hoverLabel->setBrush(Qt::white);
  hoverLabel->setZValue(100);
  scene()->addItem(hoverLabel);
}

void GraphWidget::wheelEvent(QWheelEvent *event) {
  if (event->angleDelta().y() > 0 && currentScale < maxScale) {
    scale(scaleFactor, scaleFactor);
    currentScale *= scaleFactor;
  } else if (event->angleDelta().y() < 0 && currentScale > minScale) {
    scale(1.0 / scaleFactor, 1.0 / scaleFactor);
    currentScale /= scaleFactor;
  }
  event->accept();
}

void GraphWidget::keyPressEvent(QKeyEvent *event) {
  switch (event->key()) {
  case Qt::Key_Plus:
  case Qt::Key_Equal:
    if (currentScale < maxScale) {
      scale(scaleFactor, scaleFactor);
      currentScale *= scaleFactor;
    }
    break;

  case Qt::Key_Minus:
    if (currentScale > minScale) {
      scale(1.0 / scaleFactor, 1.0 / scaleFactor);
      currentScale /= scaleFactor;
    }
    break;

  default:
    QGraphicsView::keyPressEvent(event);
  }
}

void GraphWidget::mouseMoveEvent(QMouseEvent *event) {
  QPointF scenePos = mapToScene(event->pos());

  QPointF closest;
  double minDist = std::numeric_limits<double>::max();
  for (const QPointF &p : functionData) {
    double dist = std::hypot(scenePos.x() - p.x(), scenePos.y() - p.y());
    if (dist < minDist) {
      minDist = dist;
      closest = p;
    }
  }

  if (minDist < 5.0 / currentScale) {
    hoverLabel->setText(QString("x: %1\ny: %2")
                            .arg(closest.x(), 0, 'f', 2)
                            .arg(closest.y(), 0, 'f', 2));
    hoverLabel->setPos(closest + QPointF(5, -20));
    // hoverLabel->setScale(-0.5);
    // hoverLabel->show();
  } else {
    hoverLabel->hide();
  }

  QGraphicsView::mouseMoveEvent(event);
}

void GraphWidget::mousePressEvent(QMouseEvent *event) {
  if (isMeasuringDistance) {
    QPointF scenePos = mapToScene(event->pos());

    if (!firstPointSelected) {
      firstPoint = scenePos;
      firstPointSelected = true;
      emit point1Selected(firstPoint.x(), firstPoint.y());

    } else {
      QPointF secondPoint = scenePos;
      firstPointSelected = false;
      isMeasuringDistance = false;

      emit point2Selected(secondPoint.x(), secondPoint.y());

      qreal dx = secondPoint.x() - firstPoint.x();
      qreal dy = secondPoint.y() - firstPoint.y();
      qreal distance = std::sqrt(dx * dx + dy * dy);

      if (onDistanceComputed) {
        QString result = "Расстояние: " + QString::number(distance, 'f', 4);
        onDistanceComputed(result);
      }
    }
  } else {
    QGraphicsView::mousePressEvent(event);
  }
}

void GraphWidget::startDistanceMeasurement(
    std::function<void(const QString &)> callback) {
  isMeasuringDistance = true;
  firstPointSelected = false;
  onDistanceComputed = callback;
}
