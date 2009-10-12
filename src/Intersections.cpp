#include "Intersections.h"
#include "SegmentContainer.h"
//#include <vector>
#include <algorithm>


int Segment::iXSweepLine;

//Segment::Segment(qreal x1, qreal y1, qreal x2, qreal y2) : QLineF(x1, y1, x2, y2), epLeftPoint(0), epRightPoint(0)
//{
//	epLeftPoint = new EndPoint
//}
//EndPoint * Segment::leftPoint() const
//{
//}
//
//EndPoint * Segment::rightPoint() const
//{
//}

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
Intersections::Intersections() : curEventPoint(0)
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

	/*vector<EndPoint*> *vecEndPoints = new vector<EndPoint*>;
	
	for (map<int, Segment>::const_iterator it = mapSegments.begin(); it != mapSegments.end(); ++it) {
		vecEndPoints->push_back(new EndPoint(it->second.x2(), it->second.y2(), it->first));
		vecEndPoints->push_back(new EndPoint(it->second.x1(), it->second.y1(), it->first));
		sort(vecEndPoints->begin(), vecEndPoints->end(), LexSortPoints());
		
		vecEndPoints->at(0)->setEndPointType(EndPoint::Left);
		vecEndPoints->at(1)->setEndPointType(EndPoint::Right);
		
		lstEventPoints.push_back(vecEndPoints->at(0));
		lstEventPoints.push_back(vecEndPoints->at(1));
		lstEventPoints.sort(LexSortPoints());

		vecEndPoints->clear();
	}
	delete vecEndPoints;
	
	for (list<EndPoint *>::iterator it = lstEventPoints.begin(); it != lstEventPoints.end(); ++it) {
		addToStatSweepLine(*(*it));	
	}*/
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

//void SortSegments::setSweepLine(int x) 
//{
//	qlfSweepLine->setP1(QPointF(x, 0)); 
//	qlfSweepLine->setP2(QPointF(x, 100/*maxY*/));
//}

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