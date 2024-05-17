QUrl QQuickWebView::url() const
{
    Q_D(const QQuickWebView);
    RefPtr<WebFrameProxy> mainFrame = d->webPageProxy->mainFrame();
    if (!mainFrame)
        return QUrl();
    return QUrl(QString(mainFrame->url()));
}
