void QQuickWebViewPrivate::loadDidFail(const QtWebError& error)
{
    Q_Q(QQuickWebView);
    ASSERT(!q->loading());

    QWebLoadRequest loadRequest(error.url(), QQuickWebView::LoadFailedStatus, error.description(), static_cast<QQuickWebView::ErrorDomain>(error.type()), error.errorCode());
    emit q->loadingChanged(&loadRequest);
}
