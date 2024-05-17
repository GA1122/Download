bool QQuickWebView::canGoForward() const
{
    Q_D(const QQuickWebView);
    return d->webPageProxy->canGoForward();
}
