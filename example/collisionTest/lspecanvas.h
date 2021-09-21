#ifndef LSPECANVAS_H
#define LSPECANVAS_H

#include "stable.h"

namespace Ui {
class LspeCanvas;
}

class LspeCanvas : public QWidget
{
	Q_OBJECT

public:
	explicit LspeCanvas(QWidget *parent = 0);
	~LspeCanvas();

	void setInterval(int interval);
	void start();

	//! customizable function
	void paintEvent(QPaintEvent *event);

public slots:
	void render();

private:
	Ui::LspeCanvas *ui;

	QPainter *painter;
	QTimer *tmRender;

	lspeman *man;
	bool initialized;

	//! customizable function
	static bool visit(lspe::abt::node *node, void *extra);
	void drawObject(Object *obj);
	void query(Object *obj);
	lspeman* setup();
};

#endif // LSPECANVAS_H
