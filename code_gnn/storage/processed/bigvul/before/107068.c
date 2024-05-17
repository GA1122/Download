void QQuickWebViewPrivate::setIcon(const QUrl& iconURL)
{
    Q_Q(QQuickWebView);
    if (m_iconURL == iconURL)
        return;

    String oldPageURL = QUrl::fromPercentEncoding(m_iconURL.encodedFragment());
    String newPageURL = webPageProxy->mainFrame()->url();

    if (oldPageURL != newPageURL) {
        QtWebIconDatabaseClient* iconDatabase = context->iconDatabase();
        if (!oldPageURL.isEmpty())
            iconDatabase->releaseIconForPageURL(oldPageURL);

        if (!newPageURL.isEmpty())
            iconDatabase->retainIconForPageURL(newPageURL);
    }

    m_iconURL = iconURL;
    emit q->iconChanged();
}
