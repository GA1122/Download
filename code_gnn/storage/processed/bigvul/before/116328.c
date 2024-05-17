void QQuickWebViewPrivate::updateIcon()
{
    Q_Q(QQuickWebView);

    QQuickView* view = qobject_cast<QQuickView*>(q->window());
    if (!view)
        return;

    QWebIconImageProvider* provider = static_cast<QWebIconImageProvider*>(
                view->engine()->imageProvider(QWebIconImageProvider::identifier()));
    if (!provider)
        return;

    WTF::String iconUrl = provider->iconURLForPageURLInContext(m_currentUrl, context.get());

    if (iconUrl == m_iconUrl)
        return;

    m_iconUrl = iconUrl;
    emit q->iconChanged();
}
