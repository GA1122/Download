void HTMLMediaElement::setWebLayer(WebLayer* webLayer) {
  if (webLayer == m_webLayer)
    return;

  if (!m_webLayer || !webLayer)
    setNeedsCompositingUpdate();

  if (m_webLayer)
    GraphicsLayer::unregisterContentsLayer(m_webLayer);
  m_webLayer = webLayer;
  if (m_webLayer)
    GraphicsLayer::registerContentsLayer(m_webLayer);
}
