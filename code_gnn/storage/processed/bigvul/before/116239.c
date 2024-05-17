double QQuickWebViewExperimental::devicePixelRatio() const
{
    Q_D(const QQuickWebView);
    return d->webPageProxy->deviceScaleFactor();
}
