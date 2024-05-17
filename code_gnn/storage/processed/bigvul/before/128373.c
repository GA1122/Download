LayoutRect FrameView::viewportConstrainedVisibleContentRect() const
{
    LayoutRect viewportRect = visibleContentRect();
    viewportRect.setLocation(clampScrollPosition(scrollPosition()));
    return viewportRect;
}
