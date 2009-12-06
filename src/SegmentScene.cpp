#include <QtGui>
#include "SegmentScene.h"

void SegmentScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
 {
     if (mouseEvent->button() != Qt::LeftButton)
         return;

     //DiagramItem *item;
     switch (sceneMode) {
         case InsertSegment:
             line = new SegmentItem(QLineF(mouseEvent->scenePos(), mouseEvent->scenePos()));
						 line->setFlag(QGraphicsItem::ItemIsMovable, true);
             //line->setPen(QPen(myLineColor, 2));
             addItem(line);
             break;
					default:
         ;
     }
     QGraphicsScene::mousePressEvent(mouseEvent);
 }
 
void SegmentScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
 {
     if (sceneMode == InsertSegment && line) {
			 if (this->sceneRect().contains(mouseEvent->scenePos())/*mouseEvent->scenePos().x() < this->views().first()->width()*/) {
				 QLineF newLine(line->line().p1(), mouseEvent->scenePos());
				 line->setLine(newLine);
			 }
			 else 
				 line->grabMouse();
     } else if (sceneMode == MoveSegment) {
         QGraphicsScene::mouseMoveEvent(mouseEvent);
     }
 }
 
 void SegmentScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
	if (sceneMode == InsertSegment && line) {
		QPointF tp = mouseEvent->scenePos();
		QPointF tp_1 = line->line().p1();
		QPointF tp_2 = line->line().p2();
		line->setFlag(QGraphicsItem::ItemIsSelectable, true);
    //emit segmentInserted(line->line()); // может быть, не очень хорошо вызывать сигнал, а после line = 0;
		emit segmentInserted(line);
		line = 0;
	}
	if (sceneMode == MoveSegment) {
		//SegmentItem *pLnItem;
		if (! selectedItems().empty()) {
			emit segmentMoved(dynamic_cast<SegmentItem*>(selectedItems().first()));
			//pLnItem = dynamic_cast<SegmentItem*>(selectedItems().first());
			//QPointF p = pLnItem->scenePos();
			//QLineF l = pLnItem->line();
			//l.setP1(pLnItem->mapFromScene(l.p1() += p)/*l.p1() += p*/);
			//l.setP2(pLnItem->mapFromScene(l.p2() += p)/*l.p2() += p*/);
			//pLnItem->setLine(l);
			//emit segmentMoved(pLnItem);
		}
	}
  QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

//SegmentItem* SegmentScene::item (const QLineF *cpSgm) const
//{
//	std::list<QGraphicsItem*> itemsAll = items().toStdList();
//	for (std::list<QGraphicsItem*>::const_iterator cit = itemsAll.begin(); cit != itemsAll.end(); ++cit) //наверное, можно сделать поизящнее... 
//		if (*cpSgm == dynamic_cast<SegmentItem*>(*cit)->line())
//			return dynamic_cast<SegmentItem*>(*cit);
//	return 0;
//}

SegmentItem* SegmentScene::item (int number) const
{
	std::list<QGraphicsItem*> itemsAll = items().toStdList();
	for (std::list<QGraphicsItem*>::const_iterator cit = itemsAll.begin(); cit != itemsAll.end(); ++cit)
		if ((*cit)->data(0).toInt() == number)
			return dynamic_cast<SegmentItem*>(*cit);
	return 0;
}

void SegmentItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	QGraphicsLineItem::paint(painter, option, widget);
	QPen p = pen();
	p.setWidth(4);
	//p.setColor(Qt::red);
	painter->setPen(p);
	painter->drawPoint(line().p1());
	painter->drawPoint(line().p2());
	/*painter->drawLine(line());
	if (isSelected()) {
		QPen p = pen();
		p.setStyle(Qt::DashLine);
		p.setWidth(1);
		painter->setPen(p);
		QLineF l = line();
		l.translate(0, 4.0);
		painter->drawLine(l);
		l.translate(0,-8.0);
		painter->drawLine(l);
		painter->translate(0, -4.0);
	}*/
}
