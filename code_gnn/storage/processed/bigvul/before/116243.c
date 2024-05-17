void QQuickWebViewPrivate::didReceiveServerRedirectForProvisionalLoad(const WTF::String&)
{
    Q_Q(QQuickWebView);

    q->emitUrlChangeIfNeeded();
}
