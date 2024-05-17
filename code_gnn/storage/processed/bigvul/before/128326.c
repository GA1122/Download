void FrameView::scrollPositionChanged()
{
    setWasScrolledByUser(true);

    Document* document = m_frame->document();
    document->enqueueScrollEventForNode(document);

    m_frame->eventHandler().dispatchFakeMouseMoveEventSoon();

    if (RenderView* renderView = document->renderView()) {
        if (renderView->usesCompositing())
            renderView->compositor()->frameViewDidScroll();
    }

    if (m_didScrollTimer.isActive())
        m_didScrollTimer.stop();
    m_didScrollTimer.startOneShot(resourcePriorityUpdateDelayAfterScroll, FROM_HERE);

    if (AXObjectCache* cache = m_frame->document()->existingAXObjectCache())
        cache->handleScrollPositionChanged(this);

    frame().loader().saveScrollState();
}
