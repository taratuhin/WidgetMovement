#include <QTimer>
#include <QPushButton>
#include <QRandomGenerator>
#include <QDateTime>
#include <QMouseEvent>

#include "mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    resize(300, 600);
    m_timer = new QTimer();
    m_timer->setInterval(1000);
    qsrand(QDateTime::currentMSecsSinceEpoch() / 1000);

    connect(m_timer, &QTimer::timeout,
            [this] {
                auto x = qrand() % this->width();
                auto pb = new QPushButton(this);

                pb->resize(20, 20);

                if (x > this->width() - pb->width())
                {
                    x = this->width() - pb->width();
                }

                pb->move(x, qrand() % 100);
                pb->installEventFilter(this);
                pb->show();

                auto tm = new QTimer(pb);
                tm->setInterval(qrand() % 100 + 50);

                connect(tm, &QTimer::timeout,
                        [pb, tm, this] {
                            pb->move(pb->pos() + QPoint(0, 1));

                            if(pb->pos().y() > (this->height() - pb->height()))
                            {
                                this->setStyleSheet("background-color:red;");
                                this->setWindowTitle("Game over!");
                                tm->deleteLater();
                                pb->deleteLater();
                            }
                        });

                tm->start();

                connect(pb, &QPushButton::clicked, [pb, tm] {
                    tm->deleteLater();
                    pb->deleteLater();
                });

                m_timer->setInterval(qrand() % 1000);
            });
    m_timer->start();
}

MainWindow::~MainWindow()
{
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    QEvent::Type  type = event->type();


    if (type == QEvent::HoverEnter)
    {
        auto bt = qobject_cast<QPushButton *>(obj);
        QTimer *tm = bt->findChild<QTimer *>();
        int interval = tm->interval();
        interval -= interval * 0.5;
        tm->setInterval(interval);
    }

    return QWidget::eventFilter(obj, event);
}
