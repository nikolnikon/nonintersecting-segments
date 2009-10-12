#ifndef INTERSECTIONS_H
#define INTERSECTIONS_H

#include <map>
#include <list>
#include <QLineF>

using std::list;
using std::map;

// должен представлять точку события
class EndPoint
{
public:
	enum EndPointType {Left = 0, Right, Intersection};
	EndPoint(int x, int y, short numSegment, EndPointType type) : iX(x), iY(y), siNumSegment(numSegment), pointType(type) {}
	EndPoint(int x, int y, short numSegment) : iX(x), iY(y), siNumSegment(numSegment) {};
	int x() const { return iX; }
	int y() const { return iY; }
	EndPointType type() const { return pointType; }
	short numSegment() const { return siNumSegment; }

	void setEndPointType(EndPointType type) { pointType = type; }

private:
	int iX;
	int iY;
	short siNumSegment;
	EndPointType pointType;
};

class Segment : public QLineF
{
public:
	Segment(qreal x1, qreal y1, qreal x2, qreal y2) : QLineF(x1, y1, x2, y2) {}
	//Segment(qreal x1, qreal y1, qreal x2, qreal y2); // проинициализировать qlfSweepLine нулем
	static void setXSweepLine(int xSweepLine) { iXSweepLine = xSweepLine; };
	
	static int xSweepLine() { return iXSweepLine; }
	/*const EndPoint * leftPoint() const { return epLeftPoint; }
	const EndPoint * rightPoint() const { return epRightPoint; }*/

private:
	static int iXSweepLine;
	/*EndPoint *epLeftPoint;
	EndPoint *epRightPoint;*/
	//QLineF *qlfSweepLine;

};

// должен лексиграфически сортировать точки событий по (x, y) в списке точек событий 
class LexSortPoints
{
public:
	bool operator()(const EndPoint *fstPoint, const EndPoint *sndPoint) const
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
	//void setSweepLine(int x);
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

	void addToStatSweepLine(const EndPoint *eventPoint);

private:
	map<const Segment *, const Segment *> mapIntersections;
	list<const EndPoint *> lstEventPoints;
	list<const Segment *> lstStatSweepLine;
	EndPoint *curEventPoint;
};

bool operator<(const Segment &fstSegm, const Segment &sndSegment);
//QLineF & sweepLine(int xSweepLine);

#endif