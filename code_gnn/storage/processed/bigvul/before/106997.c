void QQuickWebView::focusInEvent(QFocusEvent* event)
{
    Q_D(QQuickWebView);
    d->pageView->eventHandler()->handleFocusInEvent(event);
}
