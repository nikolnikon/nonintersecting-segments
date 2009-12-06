#include "SegmentContainer.h"

void SegmentSpace::SegmentContainer::addSegment(int x1, int y1, int x2, int y2) // возможно, стоит добавить проверку, нет ли уже такого отрезка в map
{
	/*int lastNum;
	if (! mapSegments.empty()) {
		std::map<int, Segment *>::const_iterator it = --mapSegments.end();
		lastNum = it->first;
	}
	else
		lastNum = - 1;
	
	lastNum += 1;*/
  int num = getNextNumber();
	mapSegments.insert(std::make_pair(num, new Segment(x1, y1, x2, y2, num)));
	iSegmentCount += 1;
}

int SegmentSpace::SegmentContainer::addSegment(const QLineF &crLine)
{
  int num = getNextNumber();
	std::pair<std::map<int, Segment*>::iterator, bool> res = mapSegments.insert(std::make_pair(num, new Segment(crLine, num)));
	iSegmentCount += 1;
	if (! res.second)
		return -1;
	return num;
}

int SegmentSpace::SegmentContainer::getNextNumber() const
{
  int lastNum;
	if (! mapSegments.empty()) {
		std::map<int, Segment *>::const_iterator it = --mapSegments.end();
		lastNum = it->first;
	}
	else
		lastNum = - 1;
	
	lastNum += 1;
  return lastNum;
}

const Segment * SegmentSpace::SegmentContainer::segmentByNumber(int number) const
{
	std::map<int, Segment *>::const_iterator cIt;
	cIt = mapSegments.find(number);
	if (cIt != mapSegments.end())
		return cIt->second;
	else
		return 0;
}

Segment* SegmentSpace::SegmentContainer::segmentByNumber(int number)
{
	return const_cast<Segment*>(static_cast<const SegmentContainer&>(*this).segmentByNumber(number));
}

int SegmentSpace::SegmentContainer::numberBySegment(const Segment* segment) const
{
  for (std::map<int, Segment*>::const_iterator cIt = mapSegments.begin(); cIt != mapSegments.end(); ++cIt) 
    if (cIt->second == segment) // наверное, не очень хорошо сравнивать указатели...
      return cIt->first;
  return - 1;
}

SegmentSpace::SegmentContainer & SegmentSpace::segments()
{
	static SegmentContainer sgmContainer;
	return sgmContainer;
}

//void SegmentSpace::SegmentContainer::deleteSegment(const QLineF &crLine)
//{
//	for (std::map<int, Segment*>::const_iterator cit = mapSegments.begin(); cit != mapSegments.end(); ++cit) {
//		if (*(cit->second) == crLine) {
//			delete cit->second;
//			mapSegments.erase(cit);
//			return;
//		}
//	}
//}

void SegmentSpace::SegmentContainer::deleteSegment(int number)
{
	std::map<int, Segment*>::iterator it = mapSegments.find(number);
	if (it == mapSegments.end())
		return;
	delete it->second;
	mapSegments.erase(it);
	iSegmentCount -= 1;
}

void SegmentSpace::SegmentContainer::clear()
{
	for (std::map<int, Segment* >::iterator it = mapSegments.begin(); it != mapSegments.end(); ++it)
		delete it->second;
	mapSegments.clear();
	iSegmentCount = 0;
}

SegmentSpace::SegmentContainer::~SegmentContainer()
{
	clear();	
}
