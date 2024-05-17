void QQuickWebViewPrivate::processDidCrash()
{
    pageView->eventHandler()->resetGestureRecognizers();
    QUrl url(KURL(WebCore::ParsedURLString, webPageProxy->urlAtProcessExit()));
    if (m_loadStartedSignalSent) {
        QWebLoadRequest loadRequest(url, QQuickWebView::LoadFailedStatus, QLatin1String("The web process crashed."), QQuickWebView::InternalErrorDomain, 0);
        didChangeLoadingState(&loadRequest);
    }
    qWarning("WARNING: The web process experienced a crash on '%s'.", qPrintable(url.toString(QUrl::RemoveUserInfo)));
}
