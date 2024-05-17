QQuickItem* QQuickWebViewExperimental::contentItem()
{
    Q_D(QQuickWebView);
    ASSERT(d->flickProvider);
    return d->flickProvider->contentItem();
}
