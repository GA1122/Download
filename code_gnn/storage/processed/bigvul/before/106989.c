void QQuickWebView::dragMoveEvent(QDragMoveEvent* event)
{
    Q_D(QQuickWebView);
    d->pageView->eventHandler()->handleDragMoveEvent(event);
}
