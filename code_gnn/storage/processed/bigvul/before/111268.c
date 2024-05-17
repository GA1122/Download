GraphicsLayer* WebPagePrivate::overlayLayer()
{
    if (!m_overlayLayer)
        m_overlayLayer = GraphicsLayer::create(0, this);

    return m_overlayLayer.get();
}
