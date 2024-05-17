QDeclarativeListProperty<QObject> QQuickWebViewExperimental::flickableData()
{
    Q_D(const QQuickWebView);
    ASSERT(d->flickProvider);
    return d->flickProvider->flickableData();
}
