#include "lspecanvas.h"
#include "ui_lspecanvas.h"
#include "mycircle.h"

LspeCanvas::LspeCanvas(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LspeCanvas),
    initialized(false), tmRender(nullptr), step(0),
    e(), u(-10, 10)
{
    ui->setupUi(this);
}

LspeCanvas::~LspeCanvas()
{
	if (tmRender != nullptr)
	{
		tmRender->stop();
		delete tmRender;
		tmRender = nullptr;

		for (auto &e : objects)
		{
			if (e.shape != nullptr)
			{
				delete e.shape;
				e.shape = nullptr;
			}
		}
	}
    
    delete ui;

    qDebug() << "from LspeCanvas: awsl...";
}

void LspeCanvas::AddLspeObject(Shape *pobj)
{
	if (pobj == nullptr)
	{
		qDebug() << "啥笔？滚！";
		return 0;
	}

	if (strcmp(pobj->id(), "lspe::shape::Shape") == 0)
	{
		qDebug() << "你是认真的吗？";
	} else if (strcmp(pobj->id(), "MyCircle") == 0)
	{
		_lspetype obj;
		obj.shape = pobj;

		auto p = (MyCircle*)pobj;
		auto r = p->getRadius();
		obj.motion.mass = 3.1415926 * r * r / 10;
		obj.motion.acceleration = lspe::vec2f(0, 10);
		obj.motion.velocity = lspe::vec2f(random(), random());

		objects.push_back(obj);
	}
}

void LspeCanvas::SetupLspeEnvr(float step, int interval)
{
	if (!initialized)
	{
		this->step = step;

		tmRender = new QTimer(this);
		connect(tmRender, &QTimer::timeout, this, render);
		tmRender->start(interval);
		
		initialized = true;
	}
}

void LspeCanvas::render()
{
	qDebug() << "Perform rendering...";

	for (auto &obj : objects)
	{
		if (strcmp(obj.shape->id(), "MyCircle") != 0) continue;

		lspe::vec2f dv(0, 0);
		float attenuation = 1;
		for (auto &e : objects)
		{
			if (obj.shape == e.shape) continue;
			if (strcmp(e.shape->id(), "MyCircle") != 0) continue;
			if (((MyCircle*)obj.shape)->overlap(*(Circle<float>*)e.shape))
			{
				auto v = (obj.motion.velocity * (obj.motion.mass - e.motion.mass)
					+ e.motion.velocity * e.motion.mass * 2)
					/ (obj.motion.mass + e.motion.mass);
				dv += v - obj.motion.velocity;
				attenuation *= 0.9;
			}
		}

		obj.motion.velocity += obj.motion.acceleration * step;
		obj.motion.velocity += dv;
		// obj.motion.velocity *= attenuation;

		qDebug() << " -> obj.motion.velocity = ("
			<< obj.motion.velocity.x << "," << obj.motion.velocity.y << ")";

		((MyCircle*)obj.shape)->move(obj.motion.velocity * step);

		auto radius = ((MyCircle*)obj.shape)->getRadius();
		auto position = ((MyCircle*)obj.shape)->getCenter();

		if (position.x - radius < 0)
		{
			obj.motion.velocity.x = fabs(obj.motion.velocity.x);
			obj.motion.velocity *= 0.9;
		} else if (position.x + radius > width())
		{
			obj.motion.velocity.x = -fabs(obj.motion.velocity.x);
			obj.motion.velocity *= 0.9;
		}

		if (position.y - radius < 0)
		{
			obj.motion.velocity.y = fabs(obj.motion.velocity.y);
			obj.motion.velocity *= 0.9;
		} else if (position.y + radius > height())
		{
			obj.motion.velocity.y = -fabs(obj.motion.velocity.y);
			obj.motion.velocity *= 0.9;
		}
	}

	update();
}

void LspeCanvas::paintEvent(QPaintEvent *e)
{
	if (initialized)
	{
		QPainter painter(this);
		painter.setBrush(Qt::black);
		painter.setRenderHint(QPainter::Antialiasing);

		for (auto &e : objects)
		{
			if (strcmp(e.shape->id(), "MyCircle") == 0)
			{
				auto p = (MyCircle*)e.shape;
				qDebug() << " -> drawing MyCircle...";
				auto c = p->getCenter();
				auto r = p->getRadius();
				painter.drawEllipse(c.x - r, c.y - r, r * 2, r * 2);
			}
		}
	}

	QWidget::paintEvent(e);
}

float LspeCanvas::random()
{
	return u(e);
}