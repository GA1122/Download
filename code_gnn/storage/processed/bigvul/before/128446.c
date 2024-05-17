void RenderLayerScrollableArea::setHasHorizontalScrollbar(bool hasScrollbar)
{
    if (hasScrollbar == hasHorizontalScrollbar())
        return;

    if (hasScrollbar) {
        DisableCompositingQueryAsserts disabler;
        m_hBar = createScrollbar(HorizontalScrollbar);
    } else {
        destroyScrollbar(HorizontalScrollbar);
    }

    if (m_hBar)
        m_hBar->styleChanged();
    if (m_vBar)
        m_vBar->styleChanged();

    if (box().document().hasAnnotatedRegions())
        box().document().setAnnotatedRegionsDirty(true);
}
