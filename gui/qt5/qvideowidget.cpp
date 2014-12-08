#include <QPaintEvent>
#include <QPainter>
#include "qvideowidget.hpp"

QVideoWidget::QVideoWidget(QWidget* parent)
	: QWidget(parent)
{

}

void QVideoWidget::paintEvent(QPaintEvent* e)
{
	QPainter dc(this);

	auto _size = size();
	QRect r;

	r.setTop(0);
	r.setBottom(_size.height());
	r.setLeft(0);
	r.setRight(_size.width());

	dc.drawImage(r, m_video_image);

	e->accept();


	e->accept();
}
