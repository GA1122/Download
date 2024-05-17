int QQuickWebViewExperimental::deviceWidth() const
{
    Q_D(const QQuickWebView);
    return d->webPageProxy->pageGroup()->preferences()->deviceWidth();
}
