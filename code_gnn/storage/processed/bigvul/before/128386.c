void RenderLayerScrollableArea::computeScrollDimensions()
{
    m_scrollDimensionsDirty = false;

    m_overflowRect = box().layoutOverflowRect();
    box().flipForWritingMode(m_overflowRect);

    int scrollableLeftOverflow = m_overflowRect.x() - box().borderLeft() - (box().style()->shouldPlaceBlockDirectionScrollbarOnLogicalLeft() ? box().verticalScrollbarWidth() : 0);
    int scrollableTopOverflow = m_overflowRect.y() - box().borderTop();
    setScrollOrigin(IntPoint(-scrollableLeftOverflow, -scrollableTopOverflow));
}
