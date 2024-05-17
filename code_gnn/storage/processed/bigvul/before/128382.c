RenderLayerScrollableArea::RenderLayerScrollableArea(RenderLayer& layer)
    : m_layer(layer)
    , m_inResizeMode(false)
    , m_scrollsOverflow(false)
    , m_scrollDimensionsDirty(true)
    , m_inOverflowRelayout(false)
    , m_nextTopmostScrollChild(0)
    , m_topmostScrollChild(0)
    , m_needsCompositedScrolling(false)
    , m_scrollCorner(nullptr)
    , m_resizer(nullptr)
{
    ScrollableArea::setConstrainsScrollingToContentEdge(false);

    Node* node = box().node();
    if (node && node->isElementNode()) {
        Element* element = toElement(node);
        m_scrollOffset = element->savedLayerScrollOffset();
        if (!m_scrollOffset.isZero())
            scrollAnimator()->setCurrentPosition(FloatPoint(m_scrollOffset.width(), m_scrollOffset.height()));
        element->setSavedLayerScrollOffset(IntSize());
    }

    updateResizerAreaSet();
}
