void QQuickWebViewPrivate::_q_onVisibleChanged()
{
    webPageProxy->viewStateDidChange(WebPageProxy::ViewIsVisible);
}
