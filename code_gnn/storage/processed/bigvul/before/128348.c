void FrameView::setScrollPosition(const IntPoint& scrollPoint, ScrollBehavior scrollBehavior)
{
    cancelProgrammaticScrollAnimation();
    TemporaryChange<bool> changeInProgrammaticScroll(m_inProgrammaticScroll, true);
    m_maintainScrollPositionAnchor = nullptr;

    IntPoint newScrollPosition = adjustScrollPositionWithinRange(scrollPoint);

    if (newScrollPosition == scrollPosition())
        return;

    if (scrollBehavior == ScrollBehaviorAuto) {
        RenderObject* renderer = m_frame->document()->documentElement() ? m_frame->document()->documentElement()->renderer() : 0;
        if (renderer)
            scrollBehavior = renderer->style()->scrollBehavior();
        else
            scrollBehavior = ScrollBehaviorInstant;
    }
    ScrollView::setScrollPosition(newScrollPosition, scrollBehavior);
}
