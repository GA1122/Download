void QQuickWebView::hoverLeaveEvent(QHoverEvent* event)
{
    Q_D(QQuickWebView);
    d->pageView->eventHandler()->handleHoverLeaveEvent(event);
}
