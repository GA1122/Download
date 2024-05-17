void QQuickWebViewPrivate::loadDidSucceed()
{
    Q_Q(QQuickWebView);
    ASSERT(!q->loading());

    QWebLoadRequest loadRequest(q->url(), QQuickWebView::LoadSucceededStatus);
    emit q->loadingChanged(&loadRequest);
}
