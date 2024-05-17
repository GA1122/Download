void QQuickWebViewPrivate::processDidCrash()
{
    Q_Q(QQuickWebView);

    QUrl url(KURL(WebCore::ParsedURLString, webPageProxy->urlAtProcessExit()));
    qWarning("WARNING: The web process experienced a crash on '%s'.", qPrintable(url.toString(QUrl::RemoveUserInfo)));

    pageView->eventHandler()->resetGestureRecognizers();

    if (m_loadProgress > 0 && m_loadProgress < 100) {
        QWebLoadRequest loadRequest(url, QQuickWebView::LoadFailedStatus, QLatin1String("The web process crashed."), QQuickWebView::InternalErrorDomain, 0);

        loadProgressDidChange(100);
        emit q->loadingChanged(&loadRequest);
    }
}
