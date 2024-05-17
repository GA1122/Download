void RenderLayerScrollableArea::updateScrollCornerStyle()
{
    if (!m_scrollCorner && !hasScrollbar())
        return;
    if (!m_scrollCorner && hasOverlayScrollbars())
        return;

    RenderObject* actualRenderer = rendererForScrollbar(box());
    RefPtr<RenderStyle> corner = box().hasOverflowClip() ? actualRenderer->getUncachedPseudoStyle(PseudoStyleRequest(SCROLLBAR_CORNER), actualRenderer->style()) : PassRefPtr<RenderStyle>(nullptr);
    if (corner) {
        if (!m_scrollCorner) {
            m_scrollCorner = RenderScrollbarPart::createAnonymous(&box().document());
            m_scrollCorner->setParent(&box());
        }
        m_scrollCorner->setStyle(corner.release());
    } else if (m_scrollCorner) {
        m_scrollCorner->destroy();
        m_scrollCorner = nullptr;
    }
}
