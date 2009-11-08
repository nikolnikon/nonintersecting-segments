#ifndef INTERSECTIONS_H
#define INTERSECTIONS_H

#include <map>
#include <list>
#include <QLineF>
#include "Graph.h"

// абстрактный класс точки события
class AbstractEventPoint
{
public:
	enum EventPointType {Left = 0, Right, Intersection};
	AbstractEventPoint(double x, double y, EventPointType type) : dX(x), dY(y), pointType(type) {}
	AbstractEventPoint(double x, double y) : dX(x), dY(y) {}
	virtual ~AbstractEventPoint() = 0 {}
	
	double x() const { return dX; }
	double y() const { return dY; }
	EventPointType type() const { return pointType; }

	void setEventPointType(EventPointType type) { pointType = type; }

private:
	double dX;
	double dY;
	EventPointType pointType;
};
// класс точки события, которой является конечная точка отрезка
class EndEventPoint : public AbstractEventPoint
{
public:
	EndEventPoint(double x, double y, EventPointType type, int numSegment) : AbstractEventPoint(x, y, type), iNumSegment(numSegment) {}
	EndEventPoint(double x, double y, int numSegment) : AbstractEventPoint(x, y), iNumSegment(numSegment) {}
	~EndEventPoint() {}
	int numSegment() const { return iNumSegment; }
	
private:
	int iNumSegment;
};

// класс точки события, которой является точка пересечения отрезков
class IntEventPoint : public AbstractEventPoint
{
public:
	IntEventPoint(double x, double y, EventPointType type) : AbstractEventPoint(x, y, type) {}
	IntEventPoint(double x, double y) : AbstractEventPoint(x, y) {}
	~IntEventPoint() {}

	const std::list<int> & numSegments() const { return lstNumSegments; }

	void addNumSegment(int numSegment) { lstNumSegments.push_back(numSegment); }
private:
	std::list<int> lstNumSegments;
};

class Segment : public QLineF
{
public:
	Segment(qreal x1, qreal y1, qreal x2, qreal y2, int number) : QLineF(x1, y1, x2, y2), iNum(number) {}
	IntersectType intersect (const QLineF & line, QPointF * intersectionPoint) const;
  int numSegment() const { return iNum; }

private:
	int iNum; // пока что сделал, чтобы было удобнее отлаживать, возможно, так и отсавлю
};

// должен лексиграфически сортировать точки событий по (x, y) в списке точек событий 
class LexSortPoints
{
public:
	bool operator()(const AbstractEventPoint *fstPoint, const AbstractEventPoint *sndPoint) const
	{
		if (fstPoint->x() == sndPoint->x() && fstPoint->y() == sndPoint->y())
			return false;
		
		if (fstPoint->x() != sndPoint->x()) {
			return fstPoint->x() < sndPoint->x();
		}
		else
			return fstPoint->y() < sndPoint->y(); 
	}
};

// должен сортировать отрезки, начиная с верхнего, при вставке очередного отрезка в статус заметающей прямой
class SortSegments
{
public:
	explicit SortSegments(int x);
	bool operator() (const Segment *fstSegm, const Segment *sndSegm) const
	{
		QPointF fstIPoint;
		QPointF sndIPoint;

		fstSegm->intersect(qlfSweepLine, &fstIPoint);
		sndSegm->intersect(qlfSweepLine, &sndIPoint);

		if (fstIPoint.y() == sndIPoint.y())
			return false;
		else
			return fstIPoint.y() > sndIPoint.y();
	}

private:
	QLineF qlfSweepLine;
};

// должен производить поиск всех пересечений множества отрезков
class Intersections
{
public:
	Intersections();
	~Intersections(){}
	void addEventPoint(const AbstractEventPoint *eventPoint);
	bool isInLstEventPoints(const AbstractEventPoint *eventPoint) const;
	const AbstractEventPoint* min(); 

	//void addToStatSweepLine(const EndPoint *eventPoint); // возможно, здесь лучше передавать отрезок, а не точку
	void addToStatSweepLine(const Segment *segment, int x);
	void removeFromStatSweepLine(const Segment *segment); // работает на основе find - подумать почему
	const Segment* aboveSegment(const Segment *segment) const;
	const Segment* underSegment(const Segment *segment) const;
	void swapInStatSweepLine(const Segment *cpSgm_1, const Segment *cpSgm_2);

	void findIntersections();
	void graphFromInts(GraphSpace::Graph &rGr);
	const std::multimap<const Segment*, const Segment*>& intersections() const { return mapIntersections; }
	void print() const;

private:
	std::multimap<const Segment* , const Segment* > mapIntersections; // конечный результат
	std::multimap<const Segment* , const Segment* >mapTempIntersections;
	std::list<const AbstractEventPoint*> lstEventPoints;
	std::list<const Segment*> lstStatSweepLine;
};

//bool operator<(const Segment &fstSegm, const Segment &sndSegment);

#endif