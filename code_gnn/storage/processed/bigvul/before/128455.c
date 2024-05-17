void RenderLayerScrollableArea::updateNeedsCompositedScrolling()
{
    const bool needsCompositedScrolling = layerNeedsCompositedScrolling(layer());
    if (static_cast<bool>(m_needsCompositedScrolling) != needsCompositedScrolling) {
        m_needsCompositedScrolling = needsCompositedScrolling;
        layer()->didUpdateNeedsCompositedScrolling();
    }
}
