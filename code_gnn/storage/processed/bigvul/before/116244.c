void QQuickWebViewPrivate::didRelaunchProcess()
{
    qWarning("WARNING: The web process has been successfully restarted.");

    webPageProxy->drawingArea()->setSize(viewSize(), IntSize());
    updateViewportSize();
    updateUserScripts();
}
