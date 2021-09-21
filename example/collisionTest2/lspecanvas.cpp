#include "lspecanvas.h"
#include "ui_lspecanvas.h"

LspeCanvas::LspeCanvas(QWidget *parent) :
    QWidget(parent), ui(new Ui::LspeCanvas),
    man(nullptr), shouldDrawBBox(false), initialized(false)
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

		painter->setBrush(Qt::red);

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

void LspeCanvas::query(Object *obj)
{
	struct QueryExtra
	{
		Object *_object;
		void *_extra;
		void *_userdata;
		void *_this;
	};

	void *_extra[2] = { painter, &collider };

	extra._object   = obj;
	extra._extra    = _extra;
	extra._userdata = nullptr;
	extra._this     = this;

	man->query(LspeCanvas::_query, obj->box, &extra);
}

void configCollider(
	lspe::Collider &collider,
	const Object &a, const Object &b)
{
	collider.reset();

	static lspe::collision::fnsupport supports[4] =
	{
		lspe::collision::supportLine,
		lspe::collision::supportCircle,
		lspe::collision::supportPolygen,
		lspe::collision::supportEllipse,
	};

	lspe::vec2 c1 = lspe::centroidOf(*LSPE_DETAILSHAPE(&a));
	lspe::vec2 c2 = lspe::centroidOf(*LSPE_DETAILSHAPE(&b));
	lspe::vec2 firstdirection = c1 - c2;

	collider.setTestPair(a.shape, b.shape);
	collider.bindSupports(supports[a.type], supports[b.type]);
	collider.bindInitialGenerator(
		[](lspe::Shape, lspe::Shape, const lspe::vec2&, void *extra)
		-> lspe::vec2 { return *(lspe::vec2*)extra; }
	);
	collider.bindExtraData(&firstdirection);
}

bool LspeCanvas::_query(const lspe::abt::node *node, void *extra)
{
	struct QueryExtra
	{
		Object *_object;
		void *_extra;
		void *_userdata;
		void *_this;
	};

	auto qe       = (QueryExtra*)extra;
	auto painter  = (QPainter*)((void**)(qe->_extra))[0];
	auto collider = (lspe::Collider*)((void**)(qe->_extra))[1];
	
	auto p        = qe->_object;
	auto q        = (Object*)(node->userdata);

	configCollider(*collider, *p, *q);

	static char *stype[] = { "Line", "Circle", "Polygen", "Ellipse" };

	if (p->index != q->index && collider->collided())
	{
		qDebug() << "Collision Test Results:"
			<< stype[p->type] << "x" << stype[q->type]
			<< "[" << p->index << ":" << q->index << "]";
	}

	return true;
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
	man->setStep(0.08f);

	return man;
}
