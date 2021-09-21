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
}

MainWidget::~MainWidget()
{
    delete ui;
}

LspeCanvas* MainWidget::canvas()
{
	return ui->canvas;
}
