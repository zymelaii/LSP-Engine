#include "lspecanvas.h"
#include "ui_lspecanvas.h"

LspeCanvas::LspeCanvas(QWidget *parent) :
    QWidget(parent), ui(new Ui::LspeCanvas),
    man(nullptr)
{
    ui->setupUi(this);

    tmRender = new QTimer(this);
    connect(tmRender, &QTimer::timeout, this, &LspeCanvas::render);
}

LspeCanvas::~LspeCanvas()
{
	LSPE_ASSERT(tmRender != nullptr);

	stopRender();

	delete tmRender;
	tmRender = nullptr;
	man = nullptr;
    
    delete ui;
}

void LspeCanvas::bind(lspeman *man)
{
	LSPE_ASSERT(man != nullptr);
	this->man = man;
}

void LspeCanvas::setInterval(int interval)
{
	LSPE_ASSERT(tmRender != nullptr);
	if (tmRender->isActive())
	{
		tmRender->stop();
		tmRender->start(interval);
	} else
	{
		tmRender->setInterval(interval);
	}
}

void LspeCanvas::startRender()
{
	LSPE_ASSERT(tmRender != nullptr);
	tmRender->start();
}

void LspeCanvas::stopRender()
{
	LSPE_ASSERT(tmRender != nullptr);
	tmRender->stop();
}

void LspeCanvas::render()
{
	man->stepforward();
	update();
}

void LspeCanvas::paintEvent(QPaintEvent *event)
{
	if (tmRender->isActive() && man != nullptr)
	{
		QPainter tmp(this);
		painter = &tmp;

		painter->setPen(Qt::blue);
		painter->setBrush(Qt::NoBrush);

		man->traverse((lspe::abt::fnvisit)visit, this, lspe::abt::POSTORDER);

		painter->setBrush(Qt::red);
		for (auto &e : man->getObjects())
		{
			lspe::vec2 l = e.box.lower;
			lspe::vec2 s = e.box.upper - l;
			painter->drawRect(l.x, l.y, s.x, s.y);
		}

		painter->setPen(QPen(Qt::black, 8));
		painter->setBrush(Qt::NoBrush);
		painter->drawRect(0, 0, width(), height());
		painter = nullptr;
	}

	QWidget::paintEvent(event);
}

bool LspeCanvas::visit(lspe::abt::node *node, void *extra)
{
	auto p = (LspeCanvas*)extra;
	lspe::vec2 l = node->box.lower;
	lspe::vec2 s = node->box.upper - l;
	p->painter->drawRect(l.x, l.y, s.x, s.y);
	return true;
}
