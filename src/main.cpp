#include <QApplication>
#include "SegmentContainer.h"

int main(int argc, char **argv)
{
	using SegmentSpace::segments;

	QApplication app(argc, argv);

	segments().addSegment(2, 10, 4, 7); // L1
	segments().addSegment(4, 10, 1, 7); // L2
	segments().addSegment(9, 10, 6, 8); // L3
	segments().addSegment(4, 5, 5, 2); // L4
	segments().addSegment(8, 5, 6, 2); // L5
	segments().addSegment(4, 3, 8, 3); // L6

	Intersections ints;
	
	return app.exec();
}
