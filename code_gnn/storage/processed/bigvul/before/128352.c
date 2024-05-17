void FrameView::setVisibleContentScaleFactor(float visibleContentScaleFactor)
{
    if (m_visibleContentScaleFactor == visibleContentScaleFactor)
        return;

    m_visibleContentScaleFactor = visibleContentScaleFactor;
    updateScrollbars(scrollOffset());
}
