qreal QQuickWebViewExperimental::contentWidth() const
{
    Q_D(const QQuickWebView);
    ASSERT(d->flickProvider);
    return d->flickProvider->contentWidth();
}
