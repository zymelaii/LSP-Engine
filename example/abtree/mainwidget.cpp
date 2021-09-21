#include "mainwidget.h"
#include "ui_mainwidget.h"

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);

    this->setWindowTitle("LSPE DEMO - Dynamic AABB Tree");
	this->setFixedSize(this->size());
	this->setWindowFlags(this->windowFlags() &~ Qt::WindowMinMaxButtonsHint);
}

MainWidget::~MainWidget()
{
	ui->canvas->stopRender();

    delete ui;
}

void MainWidget::bind(lspeman *man)
{
	ui->canvas->bind(man);
}

LspeCanvas* MainWidget::canvas()
{
	return ui->canvas;
}
