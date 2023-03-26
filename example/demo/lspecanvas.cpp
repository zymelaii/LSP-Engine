#include "lspecanvas.h"
#include "ui_lspecanvas.h"

LspeCanvas::LspeCanvas(QWidget *parent) :
    QWidget(parent), ui(new Ui::LspeCanvas),
    shouldDrawBBox(false), initialized(false),
    solver(10, 0.01)
{
    ui->setupUi(this);

    tmRender = new QTimer(this);
    LSPE_ASSERT(tmRender != nullptr);

    connect(tmRender, &QTimer::timeout, this, &LspeCanvas::render);

    setup();
}

LspeCanvas::~LspeCanvas()
{
	LSPE_ASSERT(tmRender != nullptr);

	tmRender->stop();
	delete tmRender;
	tmRender = nullptr;

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
	solver.preSolve();
	solver.inSolve();
	solver.postSolve();
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
		QPainter tmp(this);
		painter = &tmp;

		painter->setPen(Qt::blue);
		painter->setBrush(Qt::NoBrush);

		if (shouldDrawBBox)
		{
			solver.traverse(&LspeCanvas::visit, this, lspe::abt::POSTORDER);
		}

		painter->setRenderHint(QPainter::Antialiasing);

		painter->setBrush(QColor(255, 0, 0, 200));

		for (auto obj : solver.getObjects())
		{
			drawObject(obj);
		}

		painter->setPen(QPen(Qt::black, 8));
		painter->setBrush(Qt::NoBrush);
		painter->drawRect(0, 0, width(), height());
		painter = nullptr;

		// qDebug() << "Finish rendering";
	}

	QWidget::paintEvent(event);
}

void LspeCanvas::drawObject(lspe::RigidBody *obj)
{
	using namespace lspe::shape;

	float ratio = solver.getRatio();

	switch (obj->getShape().type)
	{
		case lspe::ShapeType::eLine:
		{
			auto e = (Line*)(obj->getShape().data);
			painter->save();
			painter->setPen(QPen(Qt::red, 2));
			vec2 pa = e->pa;
			vec2 pb = e->pb;
			painter->drawLine(pa.x, pa.y, pb.x, pb.y);
			painter->restore();
		}
		break;
		case lspe::ShapeType::eCircle:
		{
			auto e = (Circle*)(obj->getShape().data);
			vec2 center = e->center;
			float r = e->r;
			painter->drawEllipse(
				center.x - r, center.y - r, r * 2, r * 2);
		}
		break;
		case lspe::ShapeType::ePolygen:
		{
			auto e = (Polygen*)(obj->getShape().data);
			std::vector<QPointF> points(e->vertices.size());
			points.clear();
			for (auto p : e->vertices)
			{
				points.push_back(QPointF(p.x, p.y));
			}
			painter->drawPolygon(&points[0], points.size());
			// painter->drawRect(
			// 	e->vertices[0].x, e->vertices[0].y,
			// 	(e->vertices[2] - e->vertices[0]).x,
			// 	(e->vertices[2] - e->vertices[0]).y);
		}
		break;
		case lspe::ShapeType::eEllipse:
		{
			auto e = (Ellipse*)(obj->getShape().data);
			vec2 center = e->center;
			float rx = e->rx;
			float ry = e->ry;
			painter->translate(center.x, center.y);
			painter->rotate(e->rotation / lspe::Pi * 180);
			painter->translate(-center.x, -center.y);
			painter->drawEllipse(
				center.x - rx, center.y - ry,
				rx * 2, ry * 2);
			painter->resetTransform();
		}
		break;
		default: LSPE_ASSERT(false);
	}

	painter->resetTransform();
}

bool LspeCanvas::visit(const lspe::abt::node *node, void *extra)
{
	auto p = (LspeCanvas*)extra;
	lspe::vec2 l = node->box.lower;
	lspe::vec2 s = node->box.upper - l;
	p->painter->drawRect(l.x, l.y, s.x, s.y);
	return true;
}

void LspeCanvas::setup()
{
	int id;
	lspe::RigidBody *body;

	//! create a rectangle as the ground
	solver.newRectangleBody({ { 0, 540 }, { 600, 580 } });
	body = (lspe::RigidBody*)solver.getUserdata(id);
	body->setMass(sqrt(lspe::Inf));
	body->setInertia(sqrt(lspe::Inf));
	// body->applyForce2Center({ 0, -3 * body->getMass() }, true);
	// body->setBodyType(lspe::BodyType::eStatic);

	id = solver.newCircleBody({ 500, 137 }, 50);
	body = (lspe::RigidBody*)solver.getUserdata(id);
	body->applyForce2Center({ 0, 9.8f * body->getMass() }, true);
	LSPE_DEBUG("CircleBody: Mass = %f kg", body->getMass());
	LSPE_DEBUG("CircleBody: Inertia = %f kg*m^2", body->getInertia());

	id = solver.newCircleBody({ 300, 400 }, 100);
	body = (lspe::RigidBody*)solver.getUserdata(id);
	body->applyForce2Center({ 0, -9.8f * body->getMass() }, true);
	LSPE_DEBUG("CircleBody: Mass = %f kg", body->getMass());
	LSPE_DEBUG("CircleBody: Inertia = %f kg*m^2", body->getInertia());

	id = solver.newTriangleBody({ 64, 64 }, { 56, 200 }, { 200, 90 });
	body = (lspe::RigidBody*)solver.getUserdata(id);
	body->applyForce2Center({ 0, 9.8f * body->getMass() }, true);
	body->applyLinearImpulse2Center({ 8 * body->getMass(), 0 }, true);
	// body->applyAngularImpulse(lspe::Pi * 1e-10, true);
	LSPE_DEBUG("TriangleBody: Mass = %f kg", body->getMass());
	LSPE_DEBUG("TriangleBody: Inertia = %f kg*m^2", body->getInertia());
	LSPE_DEBUG("TriangleBody: AngularVelocity = %f rad/s", body->getProperty().angularVelocity);

	id = solver.newEllipseBody({ 300, 300 }, 100, 30);
	body = (lspe::RigidBody*)solver.getUserdata(id);
	body->applyForce2Center({ 0, 9.8f * body->getMass() }, true);
	body->applyAngularImpulse(lspe::Pi * 1e-10, true);
	LSPE_DEBUG("EllipseBody: Mass = %f kg", body->getMass());
	LSPE_DEBUG("EllipseBody: Inertia = %f kg*m^2", body->getInertia());
}
