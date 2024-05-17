void FrameView::scrollContentsIfNeeded()
{
    bool didScroll = !pendingScrollDelta().isZero();
    ScrollView::scrollContentsIfNeeded();
    if (didScroll)
        updateFixedElementPaintInvalidationRectsAfterScroll();
}
