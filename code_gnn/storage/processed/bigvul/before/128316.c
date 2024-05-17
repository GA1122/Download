void FrameView::scheduleRelayout()
{
    ASSERT(m_frame->view() == this);

    if (isSubtreeLayout()) {
        m_layoutSubtreeRoot->markContainingBlocksForLayout(false);
        m_layoutSubtreeRoot = 0;
    }
    if (!m_layoutSchedulingEnabled)
        return;
    if (!needsLayout())
        return;
    if (!m_frame->document()->shouldScheduleLayout())
        return;
    TRACE_EVENT_INSTANT1(TRACE_DISABLED_BY_DEFAULT("devtools.timeline"), "InvalidateLayout", "frame", m_frame.get());
    TRACE_EVENT_INSTANT1(TRACE_DISABLED_BY_DEFAULT("devtools.timeline.stack"), "CallStack", "stack", InspectorCallStackEvent::currentCallStack());
    InspectorInstrumentation::didInvalidateLayout(m_frame.get());

    if (m_hasPendingLayout)
        return;
    m_hasPendingLayout = true;

    page()->animator().scheduleVisualUpdate();
    lifecycle().ensureStateAtMost(DocumentLifecycle::StyleClean);
}
