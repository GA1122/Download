IntSize FrameView::layoutSize(IncludeScrollbarsInRect scrollbarInclusion) const
{
    return scrollbarInclusion == ExcludeScrollbars ? excludeScrollbars(m_layoutSize) : m_layoutSize;
}
