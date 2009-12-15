#ifndef SEGMENTSCENE_H
#define SEGMENTSCENE_H

#include <QGraphicsScene>
#include <QGraphicsLineItem> 

class SegmentItem : public QGraphicsLineItem
{
public:
	SegmentItem(const QLineF &line, QGraphicsItem *parent = 0);
protected:
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
};

class SegmentScene : public QGraphicsScene
{
	Q_OBJECT

public:
	enum Mode {InsertSegment, MoveSegment};
	
	SegmentScene(QObject *parent = 0) : QGraphicsScene(parent), sceneMode(SegmentScene::InsertSegment), /*leftButtonDown(false),*/ line(0) {}
	//SegmentItem* item (const QLineF *cpLine) const;
	SegmentItem* item (int number) const;

	//void deleteItem(QGraphicsItem *item); // проверить эту функцию

public slots:
	void setMode(Mode mode) { sceneMode = mode; }

signals:
  //void segmentInserted(const QLineF & );
	void segmentInserted(SegmentItem * );
	void segmentMoved(const SegmentItem * );
	//void segmentDeleted(const QLineF & );
	
protected:
	void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
	void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);

private:
	Mode sceneMode;
	//bool leftButtonDown;
	SegmentItem *line;
 };
 #endif
