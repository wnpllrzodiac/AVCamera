#ifndef QVIDEOWIDGET_H
#define QVIDEOWIDGET_H

#include <QWidget>

class QVideoWidget : public QWidget
{
    Q_OBJECT

public:
	QVideoWidget(QWidget* parent);

	QImage m_video_image;

private:

    virtual void paintEvent(QPaintEvent*);
};

#endif // QVIDEOWIDGET_H
