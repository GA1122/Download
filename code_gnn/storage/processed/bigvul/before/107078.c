void QQuickWebView::stop()
{
    Q_D(QQuickWebView);
    d->webPageProxy->stopLoading();
}
