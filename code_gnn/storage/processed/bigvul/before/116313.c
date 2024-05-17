void QQuickWebView::setUrl(const QUrl& url)
{
    Q_D(QQuickWebView);

    if (url.isEmpty())
        return;

    d->webPageProxy->loadURL(url.toString());
    emitUrlChangeIfNeeded();
}
