QQuickWebViewPrivate::~QQuickWebViewPrivate()
{
    webPageProxy->close();
}