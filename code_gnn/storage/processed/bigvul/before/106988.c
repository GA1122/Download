void QQuickWebView::dragLeaveEvent(QDragLeaveEvent* event)
{
    Q_D(QQuickWebView);
    d->pageView->eventHandler()->handleDragLeaveEvent(event);
}
