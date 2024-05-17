bool RenderLayerScrollableArea::updateAfterCompositingChange()
{
    layer()->updateScrollingStateAfterCompositingChange();
    const bool layersChanged = m_topmostScrollChild != m_nextTopmostScrollChild;
    m_topmostScrollChild = m_nextTopmostScrollChild;
    m_nextTopmostScrollChild = nullptr;
    return layersChanged;
}
