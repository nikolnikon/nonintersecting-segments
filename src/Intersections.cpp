#include "Intersections.h"
#include "SegmentContainer.h"
//#include <vector>
#include <algorithm>


int Segment::iXSweepLine;

SortSegments::SortSegments(int x)
{
	qlfSweepLine = QLineF(x, 0, x, 100/*maxY*/);
}

//SortSegments::~SortSegments() 
//{
//	if (qlfSweepLine)
//		delete qlfSweepLine;
//}

// формируем список точек событий
Intersections::Intersections() /*: curEventPoint(0)*/
{
	using SegmentSpace::segments;

	EndPoint *pEp_1, *pEp_2;
	
	for (int i = 0; i < segments().segmentCount(); ++i) {
		const Segment *cpSegment = segments().segmentByNumber(i);
		pEp_1 = new EndPoint(cpSegment->x1(), cpSegment->y1(), i);
		pEp_2 = new EndPoint(cpSegment->x2(), cpSegment->y2(), i);

		EndPoint *pLeftEndPoint = std::min(pEp_1, pEp_2, LexSortPoints());
		EndPoint *pRightEndPoint = std::max(pEp_1, pEp_2, LexSortPoints());

		pLeftEndPoint->setEndPointType(EndPoint::Left);
		pRightEndPoint->setEndPointType(EndPoint::Right);

		lstEventPoints.push_back(pLeftEndPoint);
		lstEventPoints.push_back(pRightEndPoint);
	}
	lstEventPoints.sort(LexSortPoints());
	
	for (list<const EndPoint *>::const_iterator cIt = lstEventPoints.begin(); cIt != lstEventPoints.end(); ++cIt) 
		addToStatSweepLine(*cIt);

  EndPoint *tmpPoint = new EndPoint(2, 10, 1, EndPoint::Left);
  bool tmp = isInLstEventPoints(tmpPoint);

  EndPoint tmpP = min();
  removeFromStatSweepLine(lstStatSweepLine.front());
}

void Intersections::addEventPoint(const EndPoint *eventPoint)
{
  lstEventPoints.push_back(eventPoint);
  lstEventPoints.sort(LexSortPoints());
}

EndPoint Intersections::min()
{
  EndPoint ep = *lstEventPoints.front();
  delete lstEventPoints.front(); // очищаем память, которую выделили при заполнении lstEventPoints 
  lstEventPoints.pop_front();
  return ep;
}

//bool EndPoint::operator== (const EndPoint * rhs)
//{
//  if ( iX == rhs->iX && iY == rhs->iY && siNumSegment == rhs->siNumSegment && pointType == rhs->pointType)
//    return true;
//  else
//    return false;
//}

//bool operator== (const EndPoint * lhs, const EndPoint * rhs)
//{
//  if ( lhs->x() == rhs->x() && lhs->y() == rhs->y() && lhs->numSegment() == rhs->numSegment() && lhs->type() == rhs->type())
//    return true;
//  else
//    return false;
//}

bool Intersections::isInLstEventPoints(const EndPoint *eventPoint) const
{
  /*std::list<const EndPoint *>::const_iterator cIt = std::find(lstEventPoints.begin(), lstEventPoints.end(), eventPoint);
  if (cIt == lstEventPoints.end())
    return false;
  else
    return true;*/
  return std::binary_search(lstEventPoints.begin(), lstEventPoints.end(), eventPoint, LexSortPoints());
}

void Intersections::addToStatSweepLine(const EndPoint *eventPoint)
{
	const Segment *segment = SegmentSpace::segments().segmentByNumber(eventPoint->numSegment());
	if (segment) {
		lstStatSweepLine.push_back(segment);
		lstStatSweepLine.sort(SortSegments(eventPoint->x()));
	}
	else
		return;
	/*map<int, const Segment*>::const_iterator it;
	it = mapSegments.find(eventPoint.NumSegment());
	if (it != mapSegments.end())
		mapStatSweepLine.insert(make_pair(it->first, it->second));*/
}

void Intersections::removeFromStatSweepLine(const Segment *segment)
{
  std::list<const Segment *>::iterator it = std::find(lstStatSweepLine.begin(), lstStatSweepLine.end(), segment);
  if (it != lstStatSweepLine.end())
    lstStatSweepLine.erase(it);
}

const Segment* Intersections::aboveSegment(const Segment *segment) const //получше проверить условия в if
{
  std::list<const Segment *>::const_iterator it = std::find(lstStatSweepLine.begin(), lstStatSweepLine.end(), segment);
  if (it == lstStatSweepLine.begin() || it == lstStatSweepLine.end())
    return 0;
  else
    return *(--it);
}

const Segment* Intersections::underSegment(const Segment *segment) const // получше проверить условия в if
{
  std::list<const Segment *>::const_iterator it = std::find(lstStatSweepLine.begin(), lstStatSweepLine.end(), segment);
  if (it == lstStatSweepLine.end() || ++it == lstStatSweepLine.end())
    return 0;
  else
    return *(it); // возвращаем просто it, т.к. выполнилась операция ++it в условии
}

bool operator<(const Segment &fstSegm, const Segment &sndSegm)
{
	QLineF *sweepLine = new QLineF(Segment::xSweepLine(), 0, Segment::xSweepLine(), 100); 
	
	QPointF fstIPoint;
	QPointF sndIPoint;

	fstSegm.intersect(*sweepLine, &fstIPoint);
	sndSegm.intersect(*sweepLine, &sndIPoint);

	if (fstIPoint.y() == sndIPoint.y())
		return false;
	else
		return fstIPoint.y() < sndIPoint.y();
}

// возвращает заметающую прямую
//QLineF & sweepLine(int xSweepLine)
//{
//	static QLineF sweepLine(0, 0, 0, 100);
//	sweepLine.setP1(QPOintF(xSweepLine, 0));
//	sweepLine.setP1(QPOintF(xSweepLine, 100));
//	return sweepLine;
//}