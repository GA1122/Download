QQuickWebViewFlickablePrivate::QQuickWebViewFlickablePrivate(QQuickWebView* viewport)
    : QQuickWebViewPrivate(viewport)
{
    viewport->setAcceptHoverEvents(false);
}
