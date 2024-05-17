void QQuickWebView::emitUrlChangeIfNeeded()
{
    Q_D(QQuickWebView);

    WTF::String activeUrl = d->webPageProxy->activeURL();
    if (activeUrl != d->m_currentUrl) {
        d->m_currentUrl = activeUrl;
        emit urlChanged();
    }
}
