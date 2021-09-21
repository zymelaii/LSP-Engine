#include "mainwidget.h"
#include "ui_mainwidget.h"
#include "mycircle.h"

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);

    this->setWindowTitle("LSPE DEMO - Collision");
	this->setFixedSize(this->size());
	this->setWindowFlags(this->windowFlags() &~ Qt::WindowMinMaxButtonsHint);

	using lspe::vec2f;

	std::default_random_engine e;
	std::uniform_real_distribution<float> u1(0, 600);
	std::uniform_real_distribution<float> u2(2, 16);

	for (int i = 0; i < 32; ++i)
	{
		ui->canvas->AddLspeObject(new MyCircle(vec2f(u1(e), u1(e)), u2(e)));
	}

	// ui->canvas->AddLspeObject(new MyCircle(vec2f(66, 345), 2));
	// ui->canvas->AddLspeObject(new MyCircle(vec2f(7, 3), 4));
	// ui->canvas->AddLspeObject(new MyCircle(vec2f(580, 64), 8));
	// ui->canvas->AddLspeObject(new MyCircle(vec2f(29, 59), 16));
	// ui->canvas->AddLspeObject(new MyCircle(vec2f(400, 124), 32));
	// ui->canvas->AddLspeObject(new MyCircle(vec2f(345, 500), 64));

	ui->canvas->SetupLspeEnvr(0.1f, 20);
}

MainWidget::~MainWidget()
{
    delete ui;
}
