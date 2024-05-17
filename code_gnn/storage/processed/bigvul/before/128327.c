void FrameView::scrollTo(const IntSize& newOffset)
{
    LayoutSize offset = scrollOffset();
    ScrollView::scrollTo(newOffset);
    if (offset != scrollOffset()) {
        updateLayersAndCompositingAfterScrollIfNeeded();
        scrollPositionChanged();
    }
    frame().loader().client()->didChangeScrollOffset();
}
