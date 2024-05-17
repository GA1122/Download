void QQuickWebView::mouseDoubleClickEvent(QMouseEvent* event)
{
    Q_D(QQuickWebView);
    forceActiveFocus();
    d->handleMouseEvent(event);
}
