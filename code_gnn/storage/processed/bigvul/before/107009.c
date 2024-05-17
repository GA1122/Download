void QQuickWebView::hoverMoveEvent(QHoverEvent* event)
{
    Q_D(QQuickWebView);
    d->pageView->eventHandler()->handleHoverMoveEvent(event);
}
