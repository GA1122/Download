void QQuickWebViewPrivate::loadProgressDidChange(int loadProgress)
{
    Q_Q(QQuickWebView);

    m_loadProgress = loadProgress;

    emit q->loadProgressChanged();
}
