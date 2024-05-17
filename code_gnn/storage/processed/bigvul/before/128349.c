void FrameView::setScrollPositionNonProgrammatically(const IntPoint& scrollPoint)
{
    IntPoint newScrollPosition = adjustScrollPositionWithinRange(scrollPoint);

    if (newScrollPosition == scrollPosition())
        return;

    TemporaryChange<bool> changeInProgrammaticScroll(m_inProgrammaticScroll, false);
    notifyScrollPositionChanged(newScrollPosition);
}
