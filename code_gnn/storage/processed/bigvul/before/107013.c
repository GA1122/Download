void QQuickWebViewFlickablePrivate::initialize(WKContextRef contextRef, WKPageGroupRef pageGroupRef)
{
    QQuickWebViewPrivate::initialize(contextRef, pageGroupRef);
    webPageProxy->setUseFixedLayout(true);
}
