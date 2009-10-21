#ifndef SEGMENTCONTAINER_H
#define SEGMENTCONTAINER_H

#include <map>
#include "Intersections.h"

namespace SegmentSpace
{
	class SegmentContainer
	{
	public:
		~SegmentContainer();
		void addSegment(int x1, int y1, int x2, int y2);

		const Segment * segmentByNumber(int number) const;
    int numberBySegment(const Segment *segment) const;
		int segmentCount() const { return iSegmentCount; }
		friend SegmentContainer & segments();

	private:
		//SegmentContainer(); // ��� ���������� ����� �������� ������, ���� � ������� ������ ���
		SegmentContainer() {}
		//SegmentContainer(const SegmentContainer &);
		SegmentContainer(const SegmentContainer &) {}
		std::map<int, Segment*> mapSegments;
		int iSegmentCount;
	};
	
	SegmentContainer & segments(); // ���������� ���������� ������� � ����� ����������, ����� ����������� ������ ������
}

#endif