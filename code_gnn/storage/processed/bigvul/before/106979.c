void QQuickWebViewPrivate::didChangeBackForwardList()
{
    navigationHistory->d->reset();
}
