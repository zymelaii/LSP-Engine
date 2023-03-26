#include "mainwidget.h"
#include "ui_mainwidget.h"

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);

    this->setWindowTitle("LSPE DEMO - Collision Test 2 | EPA");
	this->setFixedSize(this->size());
	this->setWindowFlags(this->windowFlags() &~ Qt::WindowMinMaxButtonsHint);

	connect(ui->cbAABB, &QCheckBox::stateChanged,
		ui->canvas, &LspeCanvas::updateShouldDrawBBox);
	connect(ui->cbCollisionResponse, &QCheckBox::stateChanged,
		ui->canvas, &LspeCanvas::updateShouldRespondCollision);

	ui->canvas->setMouseTracking(true);
}

MainWidget::~MainWidget()
{
    delete ui;
}

LspeCanvas* MainWidget::canvas()
{
	return ui->canvas;
}
