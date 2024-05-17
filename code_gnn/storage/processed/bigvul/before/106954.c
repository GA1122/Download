void QQuickWebViewPrivate::_q_onIconChangedForPageURL(const QUrl& pageURL, const QUrl& iconURL)
{
    Q_Q(QQuickWebView);
    if (q->url() != pageURL)
        return;

    setIcon(iconURL);
}
