void QQuickWebView::dropEvent(QDropEvent* event)
{
    Q_D(QQuickWebView);
    d->pageView->eventHandler()->handleDropEvent(event);
}
