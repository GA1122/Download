void QQuickWebView::loadHtml(const QString& html, const QUrl& baseUrl, const QUrl& unreachableUrl)
{
    Q_D(QQuickWebView);
    if (unreachableUrl.isValid())
        d->webPageProxy->loadAlternateHTMLString(html, baseUrl.toString(), unreachableUrl.toString());
    else
        d->webPageProxy->loadHTMLString(html, baseUrl.toString());
}
