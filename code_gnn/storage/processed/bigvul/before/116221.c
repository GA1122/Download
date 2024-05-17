void QQuickWebViewPrivate::_q_onReceivedResponseFromDownload(QWebDownloadItem* downloadItem)
{
    if (!downloadItem)
        return;

    Q_Q(QQuickWebView);
    QQmlEngine::setObjectOwnership(downloadItem, QQmlEngine::JavaScriptOwnership);
    emit q->experimental()->downloadRequested(downloadItem);
}
