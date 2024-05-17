bool QQuickWebView::canGoBack() const
{
    Q_D(const QQuickWebView);
    return d->webPageProxy->canGoBack();
}
