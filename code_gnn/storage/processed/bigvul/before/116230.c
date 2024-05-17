bool QQuickWebView::childMouseEventFilter(QQuickItem* item, QEvent* event)
{
    if (!isVisible() || !isEnabled() || !s_flickableViewportEnabled)
        return QQuickFlickable::childMouseEventFilter(item, event);

    Q_D(QQuickWebView);
    if (d->m_dialogActive) {
        event->ignore();
        return false;
    }


    switch (event->type()) {
    case QEvent::MouseButtonPress:
        mousePressEvent(static_cast<QMouseEvent*>(event));
        return event->isAccepted();
    case QEvent::MouseMove:
        mouseMoveEvent(static_cast<QMouseEvent*>(event));
        return event->isAccepted();
    case QEvent::MouseButtonRelease:
        mouseReleaseEvent(static_cast<QMouseEvent*>(event));
        return event->isAccepted();
    case QEvent::TouchBegin:
    case QEvent::TouchUpdate:
    case QEvent::TouchEnd:
        touchEvent(static_cast<QTouchEvent*>(event));
        return event->isAccepted();
    default:
        break;
    }

    return QQuickFlickable::childMouseEventFilter(item, event);
}
