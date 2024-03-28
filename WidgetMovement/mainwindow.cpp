#include <QTimer>
#include <QPushButton>
#include <QRandomGenerator>
#include <QMouseEvent>

#include "mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    resize(300, 600);
    m_timer = new QTimer();
    m_timer->setInterval(1000);

    connect(m_timer, &QTimer::timeout,
            [this] {
                auto pb = new QPushButton(this);
                pb->resize(20, 20);
                const auto x = QRandomGenerator::global()->bounded(this->width() - pb->width());

                pb->move(x, QRandomGenerator::global()->bounded(100));
                pb->installEventFilter(this);
                pb->show();

                auto tm = new QTimer(pb);
                tm->setInterval(QRandomGenerator::global()->bounded(50, 150));

                connect(tm, &QTimer::timeout,
                        [pb, this] {
                            pb->move(pb->pos() + QPoint(0, 1));

                            if(pb->pos().y() > (this->height() - pb->height()))
                            {
                                this->setStyleSheet("background-color:red;");
                                this->setWindowTitle("Game over!");
                                pb->deleteLater();
                            }
                        });

                tm->start();

                connect(pb, &QPushButton::clicked, pb, &QPushButton::deleteLater);

                m_timer->setInterval(QRandomGenerator::global()->bounded(1000));
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

        if (tm)
        {
            int interval = tm->interval();
            interval -= interval * 0.5;
            tm->setInterval(interval);
        }
    }

    return QWidget::eventFilter(obj, event);
}
