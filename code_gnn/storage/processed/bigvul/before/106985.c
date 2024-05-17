void QQuickWebViewPrivate::didRelaunchProcess()
{
    qWarning("WARNING: The web process has been successfully restarted.");
    pageView->d->setDrawingAreaSize(viewSize());
}
