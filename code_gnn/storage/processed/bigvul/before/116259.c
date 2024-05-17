void QQuickWebViewFlickablePrivate::handleMouseEvent(QMouseEvent* event)
{
    if (!pageView->eventHandler())
        return;

    pageView->eventHandler()->handleInputEvent(event);
}
