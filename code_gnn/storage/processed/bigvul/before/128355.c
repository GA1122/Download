bool FrameView::shouldSuspendScrollAnimations() const
{
    return m_frame->loader().state() != FrameStateComplete;
}
