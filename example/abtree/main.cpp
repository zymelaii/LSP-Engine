#include "stable.h"

#include "mainwidget.h"
#include <QApplication>

const int N = 64;

lspeman* setup();

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);

    lspeman* man = setup();

    MainWidget w;
    w.bind(man);
    w.show();

    w.canvas()->setInterval(40);
    w.canvas()->startRender();

    int exitCode = a.exec();

    delete man;

    return exitCode;
}

lspeman* setup() {
    auto man = new lspeman;

    man->setBBoxExtension(4.0f);
    qDebug() << "Set bbox2 extension =" << 4.0f;

    std::default_random_engine            e(time(0L));
    std::uniform_real_distribution<float> u1(0, 600);
    std::uniform_real_distribution<float> u2(4, 64);

    for (int i = 0; i < N; ++i) {
        lspe::vec2 pos(u1(e), u1(e));
        lspe::vec2 size(u2(e), u2(e));

        man->addObject(lspe::bbox2({pos, pos + size}));
        qDebug() << "Added bbox2({" << pos.x << "," << pos.y << "}, {"
                 << (pos + size).x << "," << (pos + size).y << "})";
    }

    man->setStep(0.04f);

    return man;
}
