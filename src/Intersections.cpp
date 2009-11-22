#include "Intersections.h"
#include "SegmentContainer.h"
//#include <vector>
#include <algorithm>


//int Segment::iXSweepLine;

SortSegments::SortSegments(int x)
{
	qlfSweepLine = QLineF(x, 0, x, 100/*maxY*/);
}

QLineF::IntersectType Segment::intersect (const QLineF & line, QPointF * intersectionPoint) const
{
	if (! this || ! (&line))
		return NoIntersection;
	QLineF::intersect(line, intersectionPoint);
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
	//removeFromStatSweepLine(lstStatSweepLine.front());

	/*const Segment *pS = segments().segmentByNumber(4);
	const Segment *pS_1 = underSegment(segments().segmentByNumber(4));
	const Segment *pS_2 = aboveSegment(segments().segmentByNumber(4));
	const EndEventPoint evp(0, 0, AbstractEventPoint::Left, 2);*/

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
	std::list<const Segment*>::const_iterator it = std::find(lstStatSweepLine.begin(), lstStatSweepLine.end(), segment);
	if (it == lstStatSweepLine.begin() || it == lstStatSweepLine.end())
		return 0;
	else
		return *(--it);
}

void Intersections::swapInStatSweepLine(const Segment *cpSgm_1, const Segment *cpSgm_2)
{
	std::list<const Segment* >::iterator it_1 = std::find(lstStatSweepLine.begin(), lstStatSweepLine.end(), cpSgm_1);
	std::list<const Segment* >::iterator it_2 = std::find(lstStatSweepLine.begin(), lstStatSweepLine.end(), cpSgm_2);

	std::swap(*it_1, *it_2);
}

const Segment* Intersections::underSegment(const Segment *segment) const // получше проверить условия в if
{
	std::list<const Segment *>::const_iterator it = std::find(lstStatSweepLine.begin(), lstStatSweepLine.end(), segment);
	if (it == lstStatSweepLine.end() || ++it == lstStatSweepLine.end())
		return 0;
	else
		return *(it); // возвращаем просто it, т.к. выполнилась операция ++it в условии
}

void Intersections::findIntersections()
{
	using SegmentSpace::segments;
	const AbstractEventPoint *cpEp;
	const EndEventPoint *cpEep;
	const IntEventPoint *cpIep;
	IntEventPoint *pIep;
	QPointF ptn;
	const Segment *cpSgm, *cpSgm_1, *cpSgm_2, *cpSgm_3, *cpSgm_4;
	
	while (! lstEventPoints.empty()) {
		cpEp = min();
		if (cpEp->type() == AbstractEventPoint::Left) {
			cpEep = dynamic_cast<const EndEventPoint*>(cpEp); // наверное, не очень хорошо, но пока не придумал, как сделать лучше
			cpSgm = segments().segmentByNumber(cpEep->numSegment());
			addToStatSweepLine(cpSgm, cpEep->x());
			cpSgm_1 = aboveSegment(cpSgm);
			cpSgm_2 = underSegment(cpSgm);
			
			if (cpSgm_1->intersect(*cpSgm, &ptn) == QLineF::BoundedIntersection)
				mapTempIntersections.insert(std::make_pair(cpSgm_1, cpSgm));
			if (cpSgm_2->intersect(*cpSgm, &ptn) == QLineF::BoundedIntersection)
				mapTempIntersections.insert(std::make_pair(cpSgm, cpSgm_2));
		}
		else if (cpEp->type() == AbstractEventPoint::Right) {
			cpEep = dynamic_cast<const EndEventPoint*>(cpEp);
			cpSgm = segments().segmentByNumber(cpEep->numSegment());
			cpSgm_1 = aboveSegment(cpSgm);			
			cpSgm_2 = underSegment(cpSgm);
			if (cpSgm_1->intersect(*cpSgm_2, &ptn) == QLineF::BoundedIntersection && (ptn.x() > cpEep->x()))
				mapTempIntersections.insert(std::make_pair(cpSgm_1, cpSgm_2));
			removeFromStatSweepLine(cpSgm);
		}
		else if (cpEp->type() == AbstractEventPoint::Intersection) {
			cpIep = dynamic_cast<const IntEventPoint*>(cpEp);
			std::list<int> lstSegments = cpIep->numSegments();
			if (! lstSegments.empty()) {
				cpSgm_1 = segments().segmentByNumber(lstSegments.front());
				cpSgm_2 = segments().segmentByNumber(lstSegments.back());
			}
			else
				return ;
			
			if (cpSgm_1 != aboveSegment(cpSgm_2)) {
				cpSgm_1 = cpSgm_2;
				cpSgm_2 = underSegment(cpSgm_1);
				}
			
			cpSgm_3 = aboveSegment(cpSgm_1);
			cpSgm_4 = underSegment(cpSgm_2);

			if (cpSgm_3->intersect(*cpSgm_2, &ptn) == QLineF::BoundedIntersection)
				mapTempIntersections.insert(std::make_pair(cpSgm_3, cpSgm_2));
			if (cpSgm_1->intersect(*cpSgm_4, &ptn) == QLineF::BoundedIntersection) 
				mapTempIntersections.insert(std::make_pair(cpSgm_1, cpSgm_4));
			swapInStatSweepLine(cpSgm_1, cpSgm_2);
			//lstStatSweepLine.sort(SortSegments(cpIep->x() + 1)); // думаю, что так поменяются местами cpSgm_1 и cpSgm_2
		}
		delete cpEp; // очищаем память, которую выделили при заполнении lstEventPoints
		for (std::multimap<const Segment* , const Segment* >::const_iterator cIt = mapTempIntersections.begin(); cIt != mapTempIntersections.end(); ++cIt) {
			cIt->first->intersect(*(cIt->second), &ptn);
			pIep = new IntEventPoint(ptn.x(), ptn.y(), AbstractEventPoint::Intersection);
			pIep->addNumSegment(segments().numberBySegment(cIt->first));
			pIep->addNumSegment(segments().numberBySegment(cIt->second));
			if (! isInLstEventPoints(pIep) && ! isIntersectionExist(pIep)) {
				mapIntersections.insert(std::make_pair(cIt->first, cIt->second));
				addEventPoint(pIep);
			}
			else
				delete pIep;
		}
		mapTempIntersections.clear();
	}
}

void Intersections::graphFromInts(GraphSpace::Graph& rGr)
{
	using SegmentSpace::segments;
	
	findIntersections();
	for (int i = 0; i < segments().segmentCount(); ++i)
		rGr.addNode(new GraphSpace::BaseNode(i));
	for (std::multimap<const Segment*, const Segment*>::const_iterator cIt = mapIntersections.begin(); cIt != mapIntersections.end(); ++cIt)
		rGr.addEdge(const_cast<GraphSpace::BaseNode*>(rGr.node(cIt->first->numSegment())), const_cast<GraphSpace::BaseNode*>(rGr.node(cIt->second->numSegment())));
}

bool Intersections::isIntersectionExist(const IntEventPoint *cpIep) const
{
	std::pair<std::multimap<const Segment* , const Segment*>::const_iterator, 
						std::multimap<const Segment* , const Segment*>::const_iterator> its;

	int numSgm_1 = cpIep->numSegments().front();
	int numSgm_2 = cpIep->numSegments().back();
	for (int i = 0; i < 2; ++i) {
		its = mapIntersections.equal_range(SegmentSpace::segments().segmentByNumber(numSgm_1));
		if (its.first == mapIntersections.end())
			return false;
		for (std::multimap<const Segment* , const Segment*>::const_iterator cit = its.first; cit != its.second; ++cit) {
			if (cit->second->numSegment() == numSgm_2)
				return true;
		}
		numSgm_1 = cpIep->numSegments().back();
		numSgm_2 = cpIep->numSegments().front();
	}
	return false;
}