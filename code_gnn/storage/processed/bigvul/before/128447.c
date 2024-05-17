void RenderLayerScrollableArea::setHasVerticalScrollbar(bool hasScrollbar)
{
    if (hasScrollbar == hasVerticalScrollbar())
        return;

    if (hasScrollbar) {
        DisableCompositingQueryAsserts disabler;
        m_vBar = createScrollbar(VerticalScrollbar);
    } else {
        destroyScrollbar(VerticalScrollbar);
    }

    if (m_hBar)
        m_hBar->styleChanged();
    if (m_vBar)
        m_vBar->styleChanged();

    if (box().document().hasAnnotatedRegions())
        box().document().setAnnotatedRegionsDirty(true);
}
