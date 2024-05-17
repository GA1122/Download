void LayoutSVGResourceMarker::layout()
{
    ASSERT(needsLayout());
    if (m_isInLayout)
        return;

    TemporaryChange<bool> inLayoutChange(m_isInLayout, true);

    LayoutSVGContainer::layout();

    clearInvalidationMask();
}
