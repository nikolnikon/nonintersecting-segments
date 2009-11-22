#include <QApplication>
#include "SegmentContainer.h"
#include "Graph.h"
#include "fmSegmentsImpl.h"

int main(int argc, char **argv)
{
	using SegmentSpace::segments;

	QApplication app(argc, argv);

	//segments().addSegment(2, 6, 3, 10); // L1
	//segments().addSegment(4, 10, 1, 7); // L2
	//segments().addSegment(9, 10, 6, 8); // L3
	//segments().addSegment(4, 5, 5, 2); // L4
	//segments().addSegment(8, 5, 6, 2); // L5
	//segments().addSegment(4, 3, 8, 3); // L6
  
	/*segments().addSegment(2, 10, 6, 4); // L0
	segments().addSegment(3, 10, 1, 8); // L1
	segments().addSegment(1, 7, 5, 9); // L2
	segments().addSegment(2, 6, 5, 3); // L3
	segments().addSegment(6, 7, 3, 4); // L4
	segments().addSegment(4, 9, 5, 1); // L5*/

	//segments().addSegment(2, 10, 6, 4); // L0
	//segments().addSegment(6, 4, 8, 8); // L1
	//segments().addSegment(8, 8, 12, 2); // L2
	
	//Intersections ints;
	//GraphSpace::Graph graph;
	//GraphSpace::MaxIndependentSet mis;
	//ints.findIntersections();
	//ints.graphFromInts(graph);
	//graph.print();
	//mis.findMaxIndependentSet(graph);
	fmSegmentsImpl form;
	form.show();

	return app.exec();
}
