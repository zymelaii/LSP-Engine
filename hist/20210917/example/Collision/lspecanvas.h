#ifndef LSPECANVAS_H
#define LSPECANVAS_H

#include "stable.h"

namespace Ui {
class LspeCanvas;
}

using namespace lspe::shape;

class LspeCanvas : public QWidget
{
	Q_OBJECT

public:
	explicit LspeCanvas(QWidget *parent = 0);
	~LspeCanvas();

private:
	Ui::LspeCanvas *ui;

public:	
	void AddLspeObject(Shape *obj);
	void SetupLspeEnvr(float step, int interval);
	void paintEvent(QPaintEvent *e);

public slots:
	void render();

private:
	float random();

private:
	typedef struct {
		Shape *shape;
		struct {
			float mass;
			lspe::vec2f acceleration;
			lspe::vec2f velocity;
		} motion;
	} _lspetype;

	float step;
	std::vector<_lspetype> objects;

	bool initialized;
	QTimer *tmRender;

    std::uniform_real_distribution<float> u;
	std::default_random_engine e;
};

#endif // LSPECANVAS_H
