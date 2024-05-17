void QQuickWebView::loadHtml(const QString& html, const QUrl& baseUrl)
{
    Q_D(QQuickWebView);
    d->webPageProxy->loadHTMLString(html, baseUrl.toString());
}
