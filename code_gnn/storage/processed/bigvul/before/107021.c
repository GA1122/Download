void QQuickWebViewFlickablePrivate::loadDidSucceed()
{
    if (interactionEngine)
        QQuickWebViewPrivate::loadDidSucceed();
    else
        loadSuccessDispatchIsPending = true;

}
