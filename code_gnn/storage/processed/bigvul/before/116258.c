void QQuickWebViewPrivate::handleMouseEvent(QMouseEvent* event)
{
    switch (event->type()) {
    case QEvent::MouseButtonPress:
    case QEvent::MouseButtonDblClick:
        pageView->eventHandler()->handleMousePressEvent(event);
        break;
    case QEvent::MouseMove:
        pageView->eventHandler()->handleMouseMoveEvent(event);
        break;
    case QEvent::MouseButtonRelease:
        pageView->eventHandler()->handleMouseReleaseEvent(event);
        break;
    default:
        ASSERT_NOT_REACHED();
        break;
    }
}
