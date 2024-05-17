void FrameView::scheduleRelayoutOfSubtree(RenderObject* relayoutRoot)
{
    ASSERT(m_frame->view() == this);

    if (!m_frame->document()->isActive())
        return;

    RenderView* renderView = this->renderView();
    if (renderView && renderView->needsLayout()) {
        if (relayoutRoot)
            relayoutRoot->markContainingBlocksForLayout(false);
        return;
    }

    if (layoutPending() || !m_layoutSchedulingEnabled) {
        if (m_layoutSubtreeRoot != relayoutRoot) {
            if (isObjectAncestorContainerOf(m_layoutSubtreeRoot, relayoutRoot)) {
                relayoutRoot->markContainingBlocksForLayout(false, m_layoutSubtreeRoot);
                ASSERT(!m_layoutSubtreeRoot->container() || !m_layoutSubtreeRoot->container()->needsLayout());
            } else if (isSubtreeLayout() && isObjectAncestorContainerOf(relayoutRoot, m_layoutSubtreeRoot)) {
                m_layoutSubtreeRoot->markContainingBlocksForLayout(false, relayoutRoot);
                m_layoutSubtreeRoot = relayoutRoot;
                ASSERT(!m_layoutSubtreeRoot->container() || !m_layoutSubtreeRoot->container()->needsLayout());
            } else {
                if (isSubtreeLayout())
                    m_layoutSubtreeRoot->markContainingBlocksForLayout(false);
                m_layoutSubtreeRoot = 0;
                relayoutRoot->markContainingBlocksForLayout(false);
            }
        }
    } else if (m_layoutSchedulingEnabled) {
        m_layoutSubtreeRoot = relayoutRoot;
        ASSERT(!m_layoutSubtreeRoot->container() || !m_layoutSubtreeRoot->container()->needsLayout());
        m_hasPendingLayout = true;

        page()->animator().scheduleVisualUpdate();
        lifecycle().ensureStateAtMost(DocumentLifecycle::StyleClean);
    }
    TRACE_EVENT_INSTANT1(TRACE_DISABLED_BY_DEFAULT("devtools.timeline"), "InvalidateLayout", "frame", m_frame.get());
    TRACE_EVENT_INSTANT1(TRACE_DISABLED_BY_DEFAULT("devtools.timeline.stack"), "CallStack", "stack", InspectorCallStackEvent::currentCallStack());
    InspectorInstrumentation::didInvalidateLayout(m_frame.get());
}
