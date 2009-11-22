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

void SegmentSpace::SegmentContainer::addSegment(const QLineF &crLine)
{
  int num = getNextNumber();
  mapSegments.insert(std::make_pair(num, new Segment(crLine, num)));
	iSegmentCount += 1;
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

SegmentSpace::SegmentContainer::~SegmentContainer()
{
	for (std::map<int, Segment* >::iterator it = mapSegments.begin(); it != mapSegments.end(); ++it)
		delete it->second;
	mapSegments.clear();
}
