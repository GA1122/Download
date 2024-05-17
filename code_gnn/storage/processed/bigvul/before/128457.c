void RenderLayerScrollableArea::updateResizerStyle()
{
    if (!m_resizer && !box().canResize())
        return;

    RenderObject* actualRenderer = rendererForScrollbar(box());
    RefPtr<RenderStyle> resizer = box().hasOverflowClip() ? actualRenderer->getUncachedPseudoStyle(PseudoStyleRequest(RESIZER), actualRenderer->style()) : PassRefPtr<RenderStyle>(nullptr);
    if (resizer) {
        if (!m_resizer) {
            m_resizer = RenderScrollbarPart::createAnonymous(&box().document());
            m_resizer->setParent(&box());
        }
        m_resizer->setStyle(resizer.release());
    } else if (m_resizer) {
        m_resizer->destroy();
        m_resizer = nullptr;
    }
}
