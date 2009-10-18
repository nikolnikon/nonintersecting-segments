#include "Intersections.h"
#include "SegmentContainer.h"
//#include <vector>
#include <algorithm>


int Segment::iXSweepLine;

SortSegments::SortSegments(int x)
{
	qlfSweepLine = QLineF(x, 0, x, 100/*maxY*/);
}

// формируем список точек событий
Intersections::Intersections()
{
	using SegmentSpace::segments;

	EndEventPoint *pEp_1, *pEp_2;
	
	for (int i = 0; i < segments().segmentCount(); ++i) {
		const Segment *cpSegment = segments().segmentByNumber(i);
		pEp_1 = new EndEventPoint(cpSegment->x1(), cpSegment->y1(), i);
		pEp_2 = new EndEventPoint(cpSegment->x2(), cpSegment->y2(), i);

		EndEventPoint *pLeftEndPoint = std::min(pEp_1, pEp_2, LexSortPoints());
		EndEventPoint *pRightEndPoint = std::max(pEp_1, pEp_2, LexSortPoints());

		pLeftEndPoint->setEventPointType(AbstractEventPoint::Left);
		pRightEndPoint->setEventPointType(AbstractEventPoint::Right);

		lstEventPoints.push_back(pLeftEndPoint);
		lstEventPoints.push_back(pRightEndPoint);
	}
	lstEventPoints.sort(LexSortPoints());
	
	/*for (list<const EndPoint *>::const_iterator cIt = lstEventPoints.begin(); cIt != lstEventPoints.end(); ++cIt) 
		addToStatSweepLine(*cIt);*/

  //EndPoint *tmpPoint = new EndPoint(2, 10, 1, EndPoint::Left);
  //bool tmp = isInLstEventPoints(tmpPoint);

  //EndPoint tmpP = min();
  removeFromStatSweepLine(lstStatSweepLine.front());

  const Segment *pS = segments().segmentByNumber(4);
  const Segment *pS_1 = underSegment(segments().segmentByNumber(4));
  const Segment *pS_2 = aboveSegment(segments().segmentByNumber(4));
  const EndEventPoint evp(0, 0, AbstractEventPoint::Left, 2);

}

void Intersections::addEventPoint(const AbstractEventPoint *eventPoint)
{
  lstEventPoints.push_back(eventPoint);
  lstEventPoints.sort(LexSortPoints()); // не рационально сортировать список каждый раз при вставке
}

const AbstractEventPoint* Intersections::min()
{
  const AbstractEventPoint *ep = lstEventPoints.front();
  //delete lstEventPoints.front(); // очищаем память, которую выделили при заполнении lstEventPoints 
  lstEventPoints.pop_front();
  return ep;
}

bool Intersections::isInLstEventPoints(const AbstractEventPoint *eventPoint) const
{
  return std::binary_search(lstEventPoints.begin(), lstEventPoints.end(), eventPoint, LexSortPoints());
}

//void Intersections::addToStatSweepLine(const EndPoint *eventPoint)
void Intersections::addToStatSweepLine(const Segment *segment, int x)
{
	//const Segment *segment = SegmentSpace::segments().segmentByNumber(eventPoint->numSegment());
	if (segment) {
		lstStatSweepLine.push_back(segment);
		lstStatSweepLine.sort(SortSegments(x));
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

void Intersections::findIntersections()
{
	using SegmentSpace::segments;
	const AbstractEventPoint *pEp;
	const EndEventPoint *pEep;
	const IntEventPoint *pIep;
	QPointF *pPtn;
	const Segment *pSgm, *pSgm_1, *pSgm_2, *pSgm_3, *pSgm_4;
	
	while (! lstEventPoints.empty()) {
		pEp = min();
		if (pEp->type() == AbstractEventPoint::Left) {
			pEep = dynamic_cast<const EndEventPoint*>(pEp); // наверное, не очень хорошо, но пока не придумал, как сделать лучше
			pSgm = segments().segmentByNumber(pEep->numSegment());
			addToStatSweepLine(pSgm, pEep->x());
			pSgm_1 = aboveSegment(pSgm);
			pSgm_2 = underSegment(pSgm);
			
			if (pSgm_1->intersect(*pSgm, pPtn) == QLineF::BoundedIntersection)
				mapIntersections.insert(std::make_pair(pSgm_1, pSgm));
			if (pSgm_2->intersect(*pSgm, pPtn) == QLineF::BoundedIntersection)
				mapIntersections.insert(std::make_pair(pSgm, pSgm_2));
		}
		else if (pEp->type() == AbstractEventPoint::Right) {
			pEep = dynamic_cast<const EndEventPoint*>(pEp);
			pSgm = segments().segmentByNumber(pEep->numSegment());
			pSgm_1 = aboveSegment(pSgm);			
			pSgm_2 = underSegment(pSgm);
			if (pSgm_1->intersect(*pSgm_2, pPtn) == QLineF::BoundedIntersection && (pPtn->x() > pEep->x()))
				mapIntersections.insert(std::make_pair(pSgm_1, pSgm_2));
			removeFromStatSweepLine(pSgm);
		}
		else if (pEp->type() == AbstractEventPoint::Intersection) {
			pIep = dynamic_cast<const IntEventPoint*>(pEp);
			std::list<short> lstSegments = pIep->numSegments();
			if (! lstSegments.empty()) {
				pSgm_1 = segments().segmentByNumber(lstSegments.front());
				pSgm_2 = segments().segmentByNumber(lstSegments.back());
			}
			else
				return ;
			
			if (pSgm_1 != aboveSegment(pSgm_2, pIep->x() - 1) {
				pSgm_1 = pSgm_2;
				psgm_2 = underSegment(pSgm_1, pIep->x() - 1);
			}
			
			pSgm_3 = aboveSegment(pSgm_1);
			pSgm_4 = underSegment(pSgm_2);

			if (pSgm_3->intersect(*pSgm_2, pPtn) == QLineF::BoundedIntersection)
				mapIntersections.insert(std::make_pair(pSgm_3, pSgm_2));
			if (pSgm_1->intersect(*pSgm_4, pPtn) == QLineF::BoundedIntersection) {
				mapIntersections.insert(std::make_pair(pSgm_1, pSgm_4));
				lstStatSweepLine.sort(SortSegments(pIep->x())); // думаю, что так поменяются местами pSgm_1 и pSgm_2  
		}
		delete pEp; // очищаем память, которую выделили при заполнении lstEventPoints
		for (std::map<const Segment* , const Segment* >::const_iterator cIt = mapIntersections.begin(); cIt != mapIntersections.end(); ++cIt) {
			cIt->first->intersect(*(cIt->second), pPtn);
			pIep = new IntEventPoint(pPtn->x(), pPtn->y(), AbstractEventPoint::Intersection);
			if (! isInLstEventPoints(pIep))

		}

	}
}
