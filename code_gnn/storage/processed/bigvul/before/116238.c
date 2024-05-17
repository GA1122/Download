int QQuickWebViewExperimental::deviceHeight() const
{
    Q_D(const QQuickWebView);
    return d->webPageProxy->pageGroup()->preferences()->deviceHeight();
}
