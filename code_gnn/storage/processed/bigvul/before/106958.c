void QQuickWebViewPrivate::_q_onUrlChanged()
{
    Q_Q(QQuickWebView);
    context->iconDatabase()->requestIconForPageURL(q->url());
}
