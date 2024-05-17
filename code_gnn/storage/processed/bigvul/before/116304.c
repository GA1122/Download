void QQuickWebViewExperimental::setDeviceWidth(int value)
{
    Q_D(QQuickWebView);
    d->webPageProxy->pageGroup()->preferences()->setDeviceWidth(qMax(0, value));
    emit deviceWidthChanged();
}
