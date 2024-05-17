void FrameView::updateOverflowStatus(bool horizontalOverflow, bool verticalOverflow)
{
    if (!m_viewportRenderer)
        return;

    if (m_overflowStatusDirty) {
        m_horizontalOverflow = horizontalOverflow;
        m_verticalOverflow = verticalOverflow;
        m_overflowStatusDirty = false;
        return;
    }

    bool horizontalOverflowChanged = (m_horizontalOverflow != horizontalOverflow);
    bool verticalOverflowChanged = (m_verticalOverflow != verticalOverflow);

    if (horizontalOverflowChanged || verticalOverflowChanged) {
        m_horizontalOverflow = horizontalOverflow;
        m_verticalOverflow = verticalOverflow;

        RefPtrWillBeRawPtr<OverflowEvent> event = OverflowEvent::create(horizontalOverflowChanged, horizontalOverflow, verticalOverflowChanged, verticalOverflow);
        event->setTarget(m_viewportRenderer->node());
        m_frame->document()->enqueueAnimationFrameEvent(event.release());
    }

}
