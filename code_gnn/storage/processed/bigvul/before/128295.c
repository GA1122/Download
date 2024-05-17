FrameView* FrameView::parentFrameView() const
{
    if (!parent())
        return 0;

    Frame* parentFrame = m_frame->tree().parent();
    if (parentFrame && parentFrame->isLocalFrame())
        return toLocalFrame(parentFrame)->view();

    return 0;
}
