void QQuickWebViewFlickablePrivate::_q_suspend()
{
    pageIsSuspended = true;
    webPageProxy->suspendActiveDOMObjectsAndAnimations();
}
