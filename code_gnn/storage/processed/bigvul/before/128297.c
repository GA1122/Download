void FrameView::performPostLayoutTasks()
{
    ASSERT(!isInPerformLayout());  
    TRACE_EVENT0("blink", "FrameView::performPostLayoutTasks");
    RefPtr<FrameView> protect(this);

    m_postLayoutTasksTimer.stop();

    m_frame->selection().setCaretRectNeedsUpdate();

    {
        DisableCompositingQueryAsserts disabler;
        m_frame->selection().updateAppearance();
    }

    ASSERT(m_frame->document());
    if (m_nestedLayoutCount <= 1) {
        if (m_firstLayoutCallbackPending)
            m_firstLayoutCallbackPending = false;

        if (!m_frame->document()->parsing() && m_frame->loader().stateMachine()->committedFirstRealDocumentLoad())
            m_isVisuallyNonEmpty = true;

        if (m_isVisuallyNonEmpty && !m_frame->document()->didLayoutWithPendingStylesheets() && m_firstVisuallyNonEmptyLayoutCallbackPending) {
            m_firstVisuallyNonEmptyLayoutCallbackPending = false;
            m_frame->loader().client()->dispatchDidFirstVisuallyNonEmptyLayout();
        }
    }

    FontFaceSet::didLayout(*m_frame->document());

    updateWidgetPositions();

    if (!renderView())
        return;

    scheduleUpdateWidgetsIfNecessary();

    if (Page* page = m_frame->page()) {
        if (ScrollingCoordinator* scrollingCoordinator = page->scrollingCoordinator())
            scrollingCoordinator->notifyLayoutUpdated();
    }

    scrollToAnchor();

    sendResizeEventIfNeeded();
}
