#include "SegmentContainer.h"

void SegmentSpace::SegmentContainer::addSegment(int x1, int y1, int x2, int y2) // ��������, ����� �������� ��������, ��� �� ��� ������ ������� � map
{
	int lastNum;
	if (! mapSegments.empty()) {
		std::map<int, Segment *>::const_iterator it = --mapSegments.end();
		lastNum = it->first;
	}
	else
		lastNum = - 1;
	
	mapSegments.insert(std::make_pair(++lastNum, new Segment(x1, y1, x2, y2)));
	iSegmentCount += 1;
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

SegmentSpace::SegmentContainer & SegmentSpace::segments()
{
	static SegmentContainer sgmContainer;
	return sgmContainer;
}
