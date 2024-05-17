void QQuickWebViewExperimental::setDevicePixelRatio(double devicePixelRatio)
{
    Q_D(QQuickWebView);
    if (devicePixelRatio == this->devicePixelRatio())
        return;

    d->webPageProxy->setCustomDeviceScaleFactor(devicePixelRatio);
    emit devicePixelRatioChanged();
}
