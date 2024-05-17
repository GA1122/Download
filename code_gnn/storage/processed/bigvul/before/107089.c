void QQuickWebView::wheelEvent(QWheelEvent* event)
{
    Q_D(QQuickWebView);
    d->pageView->eventHandler()->handleWheelEvent(event);
}
