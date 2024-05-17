QUrl QQuickWebView::icon() const
{
    Q_D(const QQuickWebView);
    return QUrl(d->m_iconUrl);
}
