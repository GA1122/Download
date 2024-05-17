ALWAYS_INLINE void WebFrame::traceFramesImpl(VisitorDispatcher visitor, WebFrame* frame)
{
    DCHECK(frame);
    traceFrame(visitor, frame->m_parent);
    for (WebFrame* child = frame->firstChild(); child; child = child->nextSibling())
        traceFrame(visitor, child);
    frame->m_openedFrameTracker->traceFrames(visitor);
}
