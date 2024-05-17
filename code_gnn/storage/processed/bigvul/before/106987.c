void QQuickWebView::dragEnterEvent(QDragEnterEvent* event)
{
    Q_D(QQuickWebView);
    d->pageView->eventHandler()->handleDragEnterEvent(event);
}
