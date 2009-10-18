#ifndef INTERSECTIONS_H
#define INTERSECTIONS_H

#include <map>
#include <list>
#include <QLineF>

using std::list;
using std::map;

// абстрактный класс точки события
class AbstractEventPoint
{
public:
	enum EventPointType {Left = 0, Right, Intersection};
	AbstractEventPoint(int x, int y, EventPointType type) : iX(x), iY(y), pointType(type) {}
	AbstractEventPoint(int x, int y) : iX(x), iY(y) {}
	virtual ~AbstractEventPoint() = 0 {}
	
	int x() const { return iX; }
	int y() const { return iY; }
	EventPointType type() const { return pointType; }
	//virtual short numSegment() const = 0;
	//virtual const std::list<short> & numSegments() const = 0;

	void setEventPointType(EventPointType type) { pointType = type; }

private:
	int iX;
	int iY;
	EventPointType pointType;
};
// класс точки события, которой является конечная точка отрезка
class EndEventPoint : public AbstractEventPoint
{
public:
	EndEventPoint(int x, int y, EventPointType type, short numSegment) : AbstractEventPoint(x, y, type), siNumSegment(numSegment) {}
	EndEventPoint(int x, int y, short numSegment) : AbstractEventPoint(x, y), siNumSegment(numSegment) {}
	~EndEventPoint() {}
	short numSegment() const { return siNumSegment; }
	
private:
	short siNumSegment;
};

// класс точки события, которой является точка пересечения отрезков
class IntEventPoint : public AbstractEventPoint
{
public:
	IntEventPoint(int x, int y, EventPointType type) : AbstractEventPoint(x, y, type) {}
	IntEventPoint(int x, int y) : AbstractEventPoint(x, y) {}
	~IntEventPoint() {}

	const std::list<short> & numSegments() const { return lstNumSegments; }

	void addNumSegment(short numSegment) { lstNumSegments.push_back(numSegment); }
private:
	std::list<short> lstNumSegments;
};

// должен представлять точку события
//class EndPoint
//{
//public:
//	enum EndPointType {Left = 0, Right, Intersection};
//	EndPoint(int x, int y, short numSegment, EndPointType type) : iX(x), iY(y), siNumSegment(numSegment), pointType(type) {}
//	EndPoint(int x, int y, short numSegment) : iX(x), iY(y), siNumSegment(numSegment) {};
//	int x() const { return iX; }
//	int y() const { return iY; }
//	EndPointType type() const { return pointType; }
//	short numSegment() const { return siNumSegment; }
//
//	void setEndPointType(EndPointType type) { pointType = type; }
//
//  //bool operator== (const EndPoint * rhs);
//
//private:
//	int iX;
//	int iY;
//	short siNumSegment;
//	EndPointType pointType;
//};

class Segment : public QLineF
{
public:
	Segment(qreal x1, qreal y1, qreal x2, qreal y2) : QLineF(x1, y1, x2, y2) {}
	static void setXSweepLine(int xSweepLine) { iXSweepLine = xSweepLine; };
	
	static int xSweepLine() { return iXSweepLine; }

private:
	static int iXSweepLine;
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
			bool tmp = fstPoint->x() < sndPoint->x();
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
	//~SortSegments();
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

	void findIntersections();

private:
	map<const Segment* , const Segment* > mapIntersections; // конечный результат
	//map<const Segment* , const Segment* >mapTempIntersections;
	list<const AbstractEventPoint*> lstEventPoints;
	list<const Segment*> lstStatSweepLine;
};

bool operator<(const Segment &fstSegm, const Segment &sndSegment);

#endif