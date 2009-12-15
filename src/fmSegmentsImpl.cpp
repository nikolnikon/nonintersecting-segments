#include "fmSegmentsImpl.h"
#include "SegmentContainer.h"
#include <QGraphicsLineItem>
#include <QtGui>


fmSegmentsImpl::fmSegmentsImpl() : scene(0)/*, ints(0), graph(0), maxIndSet(0)*/
{
	using SegmentSpace::segments;
	setupUi(this);
	createActions();
  //createMenus();

	scene = new SegmentScene;
	QPointF pnt = QPointF(view->mapToScene(QPoint(0, 0)));
	scene->setSceneRect(QRectF(- pnt.x(), - pnt.y(), view->width() - 5, view->height() - 5));
	createToolbars();
	view->setScene(scene);
	view->show();
	//connect(scene, SIGNAL(segmentInserted(const QLineF & )), this, SLOT(addSegment(const QLineF & )));
	connect(scene, SIGNAL(segmentInserted(SegmentItem * )), this, SLOT(addSegment(SegmentItem * )));
	connect(scene, SIGNAL(segmentMoved(const SegmentItem * )), this, SLOT(changeSegment(const SegmentItem * )));
	//connect(pbOk, SIGNAL(clicked()), this, SLOT(findSegments()));
}

void fmSegmentsImpl::pointerGroupClicked(int)
{	
	//
	SegmentSpace::segments();
	scene->setMode(SegmentScene::Mode(pointerTypeGroup->checkedId()));
}

void fmSegmentsImpl::createActions()
{
	/*deleteAction = new QAction(QIcon("./images/delete.png"), tr("&Удалить"), this);
  deleteAction->setShortcut(tr("Удалить"));
  deleteAction->setStatusTip(tr("Удалить отрезок"));
  connect(deleteAction, SIGNAL(triggered()), this, SLOT(deleteSegment()));*/

	startAction = new QAction("Найти", this);
	startAction->setShortcut(tr("Непересекающиеся отрезки"));
	startAction->setStatusTip(tr("Найти непересекающиеся отрезки"));
	connect(startAction, SIGNAL(triggered()), this, SLOT(findSegments()));

	clearAction = new QAction("Очистить", this);
	clearAction->setShortcut(tr("Очистить"));
	clearAction->setStatusTip(tr("Очистить все"));
	connect(clearAction, SIGNAL(triggered()), this, SLOT(clear()));
}

//void fmSegmentsImpl::createMenus()
//{
//	itemMenu = menuBar()->addMenu(tr("&Item"));
//  itemMenu->addAction(deleteAction);
//}

void fmSegmentsImpl::createToolbars()
{
	mainToolBar = addToolBar(tr("Start"));
	mainToolBar->addAction(startAction);

	editToolBar = addToolBar(tr("Edit"));
  //editToolBar->addAction(deleteAction);
	editToolBar->addAction(clearAction);

	QToolButton *pointerButton = new QToolButton;
  pointerButton->setCheckable(true);
  pointerButton->setIcon(QIcon("./images/pointer.png"));
  QToolButton *linePointerButton = new QToolButton;
  linePointerButton->setCheckable(true);
	linePointerButton->setChecked(true);
  linePointerButton->setIcon(QIcon("./images/linepointer.png"));

  pointerTypeGroup = new QButtonGroup;
  pointerTypeGroup->addButton(pointerButton, int(SegmentScene::MoveSegment));
  pointerTypeGroup->addButton(linePointerButton, int(SegmentScene::InsertSegment));
  connect(pointerTypeGroup, SIGNAL(buttonClicked(int )), this, SLOT(pointerGroupClicked(int )));

	pointerToolbar = addToolBar(tr("Pointer type"));
  pointerToolbar->addWidget(pointerButton);
  pointerToolbar->addWidget(linePointerButton);
}

void fmSegmentsImpl::findSegments()
{
	//QLineF tl = dynamic_cast<QGraphicsLineItem*>(scene->items().first())->line();
	//QPointF tp = dynamic_cast<QGraphicsLineItem*>(scene->items().first())->scenePos();
	if (! SegmentSpace::segments().segmentCount()) {
		return;
	}
	std::auto_ptr<Intersections> pInts(new Intersections);
	std::auto_ptr<GraphSpace::Graph> pGraph(new GraphSpace::Graph);
	std::auto_ptr<GraphSpace::MaxIndependentSet> pMaxIndSet(new GraphSpace::MaxIndependentSet);
	std::list<int> resSegments;

	pInts->graphFromInts(*pGraph);
	pMaxIndSet->findMaxIndependentSet(*pGraph, resSegments);

	for (std::list<int>::const_iterator cit = resSegments.begin(); cit != resSegments.end(); ++cit) {
		scene->item(*cit)->setPen(QPen(Qt::green, 3));
	}
}

//void fmSegmentsImpl::addSegment(const QLineF &crLine)
void fmSegmentsImpl::addSegment(SegmentItem *pLnItem)
{
	int num = SegmentSpace::segments().addSegment(pLnItem->line());
	if (num)
		pLnItem->setData(0, num);
	scene->setMode(SegmentScene::Mode(pointerTypeGroup->checkedId()));
}

void fmSegmentsImpl::changeSegment(const SegmentItem *cpLnItem)
{
	
	QPointF p = cpLnItem->scenePos();
	QLineF l = cpLnItem->line();
	SegmentSpace::segments().segmentByNumber(cpLnItem->data(0).toInt())->setPoints(l.p1() += p, l.p2() += p);
}

void fmSegmentsImpl::deleteSegment()
{
	foreach (QGraphicsItem *item, scene->selectedItems()) {
		SegmentSpace::segments().deleteSegment(item->data(0).toInt()/*dynamic_cast<QGraphicsLineItem*>(item)->line()*/);
		scene->removeItem(item);
		delete item;
		/*if (item)
			delete item;*/
	}
}
void fmSegmentsImpl::clear()
{
	SegmentSpace::segments().clear();
	scene->clear();
}

fmSegmentsImpl::~fmSegmentsImpl()
{
	delete scene;
	//delete deleteAction;
  //delete itemMenu;
  delete editToolBar;
  delete pointerToolbar;
  delete pointerTypeGroup;
}