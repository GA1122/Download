void FrameView::didScrollTimerFired(Timer<FrameView>*)
{
    if (m_frame->document() && m_frame->document()->renderView()) {
        ResourceLoadPriorityOptimizer::resourceLoadPriorityOptimizer()->updateAllImageResourcePriorities();
    }
}
