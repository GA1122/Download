void QQuickWebViewPrivate::_q_onIconChangedForPageURL(const QString& pageUrl)
{
    if (pageUrl != QString(m_currentUrl))
        return;

    updateIcon();
}
