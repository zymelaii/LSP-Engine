#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include "stable.h"
#include "lspecanvas.h"

namespace Ui {
class MainWidget;
}

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = 0);
    ~MainWidget();

    void bind(lspeman *man);
   	LspeCanvas* canvas();

private:
    Ui::MainWidget *ui;
};

#endif // MAINWIDGET_H
