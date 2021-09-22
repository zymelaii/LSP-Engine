#include "mainwidget.h"
#include "ui_mainwidget.h"

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);

    this->setWindowTitle("LSPE DEMO - Collision Test");
	this->setFixedSize(this->size());
	this->setWindowFlags(this->windowFlags() &~ Qt::WindowMinMaxButtonsHint);

	connect(ui->cbAABB, QCheckBox::stateChanged,
		ui->canvas, LspeCanvas::updateShouldDrawBBox);

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
