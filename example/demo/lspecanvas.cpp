#include "lspecanvas.h"
#include "ui_lspecanvas.h"

LspeCanvas::LspeCanvas(QWidget *parent) :
    QWidget(parent), ui(new Ui::LspeCanvas),
    man(nullptr), shouldDrawBBox(false), initialized(false),
    ondrag(false), selection(nullptr),
    shouldBack(false), enableResponse(false)
{
    ui->setupUi(this);

    man = setup();
    LSPE_ASSERT(man != nullptr);

    tmRender = new QTimer(this);
    LSPE_ASSERT(tmRender != nullptr);

    connect(tmRender, &QTimer::timeout, this, render);
}

LspeCanvas::~LspeCanvas()
{
	LSPE_ASSERT(tmRender != nullptr);
	LSPE_ASSERT(man != nullptr);

	tmRender->stop();
	delete tmRender;
	tmRender = nullptr;

	delete man;
	man = nullptr;

	delete ui;
}

void LspeCanvas::setInterval(int interval)
{
	LSPE_ASSERT(tmRender != nullptr);
	tmRender->setInterval(interval);
}

void LspeCanvas::start()
{
	LSPE_ASSERT(tmRender != nullptr);
	tmRender->start();
	initialized = true;
}

void LspeCanvas::render()
{
	man->stepforward();
	// qDebug() << "Finish stepforward()";
	update();
}

void LspeCanvas::updateShouldDrawBBox(int status)
{
	shouldDrawBBox = status;
}

void LspeCanvas::paintEvent(QPaintEvent *event)
{
	if (initialized)
	{
		// qDebug() << "Begin rendering";

		QPainter tmp(this);
		painter = &tmp;

		painter->setPen(Qt::blue);
		painter->setBrush(Qt::NoBrush);

		if (shouldDrawBBox)
		{
			man->traverse((lspe::abt::fnvisit)visit, this, lspe::abt::POSTORDER);
		}
		// qDebug() << "Finish traverse(fnvisit, void*, int)";

		painter->setRenderHint(QPainter::Antialiasing);

		painter->setBrush(QColor(255, 0, 0, 200));

		for (auto obj : man->getObjects())
		{
			if (obj->index == -1) continue;
			drawObject(obj);
			query(obj);
		}

		painter->setPen(QPen(Qt::black, 8));
		painter->setBrush(Qt::NoBrush);
		painter->drawRect(0, 0, width(), height());
		painter = nullptr;

		// qDebug() << "Finish rendering";
	}

	QWidget::paintEvent(event);
}

void LspeCanvas::drawObject(Object *obj)
{
	using namespace lspe::shape;
	switch (obj->type)
	{
		case LINE:
		{
			auto e = (Line*)obj->shape;
			painter->save();
			painter->setPen(QPen(Qt::red, 2));
			painter->drawLine(e->pa.x, e->pa.y, e->pb.x, e->pb.y);
			painter->setPen(Qt::black);
			auto center = lspe::centroidOf(*e);
			painter->drawText(center.x, center.y,
				QString::number(obj->index));
			painter->restore();
		}
		break;
		case CIRCLE:
		{
			auto e = (Circle*)obj->shape;

			// qDebug() << "draw Circle [" << obj->shape << "]: center=("
			// 	<< e->center.x << "," << e->center.y << ") r =" << e->r;

			painter->drawEllipse(
				e->center.x - e->r, e->center.y - e->r, e->r * 2, e->r * 2);
			painter->save();
			painter->setPen(Qt::black);
			painter->drawText(e->center.x, e->center.y,
				QString::number(obj->index));
			painter->restore();
		}
		break;
		case POLYGEN:
		{
			auto e = (Polygen*)obj->shape;
			painter->drawRect(
				e->vertices[0].x, e->vertices[0].y,
				(e->vertices[2] - e->vertices[0]).x,
				(e->vertices[2] - e->vertices[0]).y);
			// painter->drawPolygon(
			// 	(QPointF*)&e->vertices[0], e->vertices.size());
			painter->save();
			painter->setPen(Qt::black);
			painter->drawText(e->center.x, e->center.y,
				QString::number(obj->index));
			painter->restore();
		}
		break;
		case ELLIPSE:
		{
			auto e = (Ellipse*)obj->shape;

			// qDebug() << "draw Ellipse [" << obj->shape << "]: center=("
			// 	<< e->center.x << "," << e->center.y << ") r=("
			// 	<< e->rx << "," << e->ry << ") rotation ="
			// 	<< e->rotation / lspe::Pi * 180 << "degree";

			painter->translate(e->center.x, e->center.y);
			painter->rotate(e->rotation / lspe::Pi * 180);
			painter->translate(-e->center.x, -e->center.y);
			painter->drawEllipse(
				e->center.x - e->rx, e->center.y - e->ry,
				e->rx * 2, e->ry * 2);
			painter->resetTransform();
			painter->save();
			painter->setPen(Qt::black);
			painter->drawText(e->center.x, e->center.y,
				QString::number(obj->index));
			painter->restore();
		}
		break;
		default: LSPE_ASSERT(false);
	}
}

bool querySelection(const lspe::abt::node *node, void *extra)
{
	using namespace lspe::shape;

	struct QueryExtra
	{
		Object    **_selection;
		bool       *_ondrag;
		lspe::vec2 *_point;
	};

	auto  qe = (QueryExtra*)extra;
	auto obj = (Object*)(node->userdata);

	LSPE_DEBUG("Execute querySelection; target index=%d;", obj->index);

	bool hit = false;
	switch (obj->type)
	{
		case LINE:    hit = false; break;
		case CIRCLE:  hit = lspe::contain(*(Circle *)(obj->shape), *qe->_point); break;
		case POLYGEN: hit = lspe::contain(*(Polygen*)(obj->shape), *qe->_point); break;
		case ELLIPSE: hit = lspe::contain(*(Ellipse*)(obj->shape), *qe->_point); break;
		default: LSPE_ASSERT(false);
	}

	static const char *ShapeName[] = { "Circle", "Polygen", "Ellipse" };

	if (hit)
	{
		LSPE_DEBUG("Execute querySelection: %s[%d] has been selected",
			ShapeName[obj->type - CIRCLE],
			obj->index);

		*qe->_selection = obj;
		*qe->_ondrag    = true;
		return false;
	}

	LSPE_DEBUG("Execute querySelection: no selection");

	return true;
}

void LspeCanvas::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		LSPE_ASSERT(!ondrag);

		lspe::vec2 point(event->x(), event->y());
		LSPE_DEBUG("Entry mousePressEvent: Point(%f,%f)",
			point.x, point.y);

		struct QueryExtra
		{
			Object    **_selection;
			bool       *_ondrag;
			lspe::vec2 *_point;
		} extra;

		extra._selection = &selection;
		extra._ondrag    = &ondrag;
		extra._point     = &point;

		man->query(querySelection, point, &extra);

		if (ondrag)
		{
			precoord = point;
		}
	}
}

void LspeCanvas::mouseMoveEvent(QMouseEvent *event)
{
	if (ondrag)
	{
		LSPE_ASSERT(selection != nullptr);

		lspe::vec2 point(event->x(), event->y());

		auto displacement = point - precoord;
		man->moveObject(selection->index, selection->box, displacement);

		if (enableResponse)
		{
			query(selection);
			if (shouldBack)
			{
				man->moveObject(selection->index, selection->box, -displacement);
				shouldBack = false;
				LSPE_DEBUG("Drag Object: rejected movement");
			} else
			{
				man->translate(selection->shape, selection->type, displacement);
				precoord = point;
			}
		} else
		{
			man->translate(selection->shape, selection->type, displacement);
			precoord = point;
		}

		update();
	}
}

bool LspeCanvas::visit(lspe::abt::node *node, void *extra)
{
	auto p = (LspeCanvas*)extra;
	lspe::vec2 l = node->box.lower;
	lspe::vec2 s = node->box.upper - l;
	p->painter->drawRect(l.x, l.y, s.x, s.y);
	return true;
}

lspeman* LspeCanvas::setup()
{
	auto man = new lspeman;

	man->setBBoxExtension(4.0f);

	// man->newLine();

	man->newPolygen();
	man->newPolygen();
	man->newPolygen();
	man->newPolygen();

	man->newCircle();
	man->newCircle();

	man->newEllipse();
	man->newEllipse();

	man->setStep(0.08f);

	return man;
}
