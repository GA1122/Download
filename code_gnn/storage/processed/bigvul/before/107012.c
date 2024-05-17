void QQuickWebViewLegacyPrivate::initialize(WKContextRef contextRef, WKPageGroupRef pageGroupRef)
{
    QQuickWebViewPrivate::initialize(contextRef, pageGroupRef);
    enableMouseEvents();

    _q_onVisibleChanged();
}
