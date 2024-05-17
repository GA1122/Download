void QQuickWebViewPrivate::_q_onReceivedResponseFromDownload(QWebDownloadItem* downloadItem)
{
    if (!downloadItem)
        return;

    Q_Q(QQuickWebView);
    QDeclarativeEngine::setObjectOwnership(downloadItem, QDeclarativeEngine::JavaScriptOwnership);
    emit q->experimental()->downloadRequested(downloadItem);
}
