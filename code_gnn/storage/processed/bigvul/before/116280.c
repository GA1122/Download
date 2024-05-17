void QQuickWebViewPrivate::provisionalLoadDidStart(const WTF::String& url)
{
    Q_Q(QQuickWebView);

    q->emitUrlChangeIfNeeded();

    QWebLoadRequest loadRequest(QString(url), QQuickWebView::LoadStartedStatus);
    emit q->loadingChanged(&loadRequest);
}
