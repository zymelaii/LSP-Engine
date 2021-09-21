#ifndef LSPECANVAS_H
#define LSPECANVAS_H

#include "stable.h"
#include "myshape.h"

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
	void updateShouldDrawBBox(int status);

private:
	Ui::LspeCanvas *ui;

	QPainter *painter;
	QTimer *tmRender;

	lspeman *man;
	bool shouldDrawBBox;
	bool initialized;

	lspe::Collider collider;

	//! customizable function
	static bool visit(lspe::abt::node *node, void *extra);
	void drawObject(Object *obj);
	void query(Object *obj);
	static bool _query(const lspe::abt::node *node, void *extra);
	lspeman* setup();
};

#endif // LSPECANVAS_H
