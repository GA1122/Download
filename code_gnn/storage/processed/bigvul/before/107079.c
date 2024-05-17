QString QQuickWebView::title() const
{
    Q_D(const QQuickWebView);
    return d->webPageProxy->pageTitle();
}
