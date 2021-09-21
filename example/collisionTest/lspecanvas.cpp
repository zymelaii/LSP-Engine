#include "lspecanvas.h"
#include "ui_lspecanvas.h"

struct myQueryExtra { int last[2]; Object *obj; };
myQueryExtra extra = { { -1, -1 }, nullptr };

LspeCanvas::LspeCanvas(QWidget *parent) :
    QWidget(parent), ui(new Ui::LspeCanvas),
    man(nullptr), initialized(false)
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

void LspeCanvas::paintEvent(QPaintEvent *event)
{
	if (initialized)
	{
		// qDebug() << "Begin rendering";

		QPainter tmp(this);
		painter = &tmp;

		painter->setPen(Qt::blue);
		painter->setBrush(Qt::NoBrush);

		man->traverse((lspe::abt::fnvisit)visit, this, lspe::abt::POSTORDER);
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
	extra.obj = obj;
	man->query([](const lspe::abt::node *node, void *extra)
	-> bool {
		using namespace lspe::shape;

		char *stype[] = { "Line", "Circle", "Polygen", "Ellipse" };
		auto e = (myQueryExtra*)extra;
		auto p = e->obj;
		auto q = (Object*)(node->userdata);
		if (p->index != q->index)
		{
			lspe::collision::fnsupport supports[4] = {
				lspe::collision::supportLine,
				lspe::collision::supportCircle,
				lspe::collision::supportPolygen,
				lspe::collision::supportEllipse,
			};

			lspe::vec2 c1, c2;
			switch (p->type)
			{
				case LINE:
					c1 = lspe::centroidOf(*(Line*)(p->shape));
				break;
				case CIRCLE:
					c1 = ((Circle*)(p->shape))->center;
				break;
				case POLYGEN:
					c1 = lspe::centroidOf(*(Polygen*)(p->shape));
				break;
				case ELLIPSE:
					c1 = ((Ellipse*)(p->shape))->center;
				break;
			}
			switch (q->type)
			{
				case LINE:
					c2 = lspe::centroidOf(*(Line*)(q->shape));
				break;
				case CIRCLE:
					c2 = ((Circle*)(q->shape))->center;
				break;
				case POLYGEN:
					c2 = lspe::centroidOf(*(Polygen*)(q->shape));
				break;
				case ELLIPSE:
					c2 = ((Ellipse*)(q->shape))->center;
				break;
			}
			lspe::vec2 firstdirection = c1 - c2;

			lspe::Collider collider;
			collider.setTestPair(p->shape, q->shape);
			collider.bindSupports(supports[p->type], supports[q->type]);
			collider.bindInitialGenerator(
				[](lspe::Shape, lspe::Shape, const lspe::vec2&, void *extra)
				-> lspe::vec2 { return *(lspe::vec2*)extra; }
			);
			collider.bindExtraData((void*)&firstdirection);

			if (collider.collided())
			{
				if (p->index == e->last[0] && q->index == e->last[1]
					|| p->index == e->last[1] && q->index == e->last[0])
				{

				} else
				{
					qDebug() << "Collision Test Results:"
						<< stype[p->type] << "x" << stype[q->type]
						<< "[" << p->index << ":" << q->index << "]";
					e->last[0] = p->index;
					e->last[1] = q->index;
				}
			}
		}
		return true;
	}, obj->box, (void*)&extra);
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
	qDebug() << "Set bbox2 extension =" << 4.0f;

	{	//! Line
		const size_t N = 4;
		for (int i = 0; i < N; ++i)
		{
			man->newLine();
		}
		qDebug() << "Added" << N << "Lines";
	}

	// {	//! CIRCLE
	// 	const size_t N = 4;
	// 	for (int i = 0; i < N; ++i)
	// 	{
	// 		man->newCircle();
	// 	}
	// 	qDebug() << "Added" << N << "Circles";
	// }

	{	//! Polygen
		const size_t N = 16;
		for (int i = 0; i < N; ++i)
		{
			man->newPolygen();
		}
		qDebug() << "Added" << N << "Polygens";
	}

	{	//! Ellipse
		const size_t N = 4;
		for (int i = 0; i < N; ++i)
		{
			man->newEllipse();
		}
		qDebug() << "Added" << N << "Ellipses";
	}

	man->setStep(0.08f);

	return man;
}
