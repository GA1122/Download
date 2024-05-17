void QQuickWebViewFlickablePrivate::updateViewportSize()
{
    if (m_viewportHandler)
        m_viewportHandler->viewportItemSizeChanged();
}
