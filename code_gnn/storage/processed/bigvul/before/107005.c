void QQuickWebViewPrivate::handleDownloadRequest(DownloadProxy* download)
{
    Q_Q(QQuickWebView);
    QWebDownloadItem* downloadItem = new QWebDownloadItem();
    downloadItem->d->downloadProxy = download;

    q->connect(downloadItem->d, SIGNAL(receivedResponse(QWebDownloadItem*)), q, SLOT(_q_onReceivedResponseFromDownload(QWebDownloadItem*)));
    context->downloadManager()->addDownload(download, downloadItem);
}
