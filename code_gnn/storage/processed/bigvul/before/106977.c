PassOwnPtr<DrawingAreaProxy> QQuickWebViewPrivate::createDrawingAreaProxy()
{
    return DrawingAreaProxyImpl::create(webPageProxy.get());
}
