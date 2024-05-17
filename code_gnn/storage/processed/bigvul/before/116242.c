void QQuickWebViewFlickablePrivate::didChangeViewportProperties(const WebCore::ViewportAttributes& newAttributes)
{
    if (m_viewportHandler)
        m_viewportHandler->viewportAttributesChanged(newAttributes);
}
