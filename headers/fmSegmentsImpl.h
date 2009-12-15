#ifndef FMSEGMENTSIMPL_H
#define FMSEGMENTSIMPL_H

#include "ui_fmSegments.h"
#include "Intersections.h"
#include "Graph.h"
#include "SegmentScene.h"

class fmSegmentsImpl : public QMainWindow, private Ui::fmSegments
{
	Q_OBJECT

public:
	fmSegmentsImpl();
	~fmSegmentsImpl();

private slots:
	void findSegments();
  //void addSegment(const QLineF &crLine);
	void addSegment(SegmentItem *pLnItem);
	void changeSegment(const SegmentItem *cpLnItem);
	void deleteSegment();
	void clear();
	void pointerGroupClicked(int id);

private:
	void createActions();
  //void createMenus();
  void createToolbars();
	
	SegmentScene *scene;
	//QAction *deleteAction;
	QAction *startAction;
	QAction *clearAction;
	//QMenu *itemMenu;
	QToolBar *mainToolBar;
	QToolBar *editToolBar;
	QToolBar *pointerToolbar;
	QButtonGroup *pointerTypeGroup;
	//Intersections *ints;
	//GraphSpace::Graph *graph;
	//GraphSpace::MaxIndependentSet *maxIndSet;
};
#endif

//class SegmentGraphicsItem : public QLineGraphicsItem