ALWAYS_INLINE void WebFrame::traceFrameImpl(VisitorDispatcher visitor, WebFrame* frame)
{
    if (!frame)
        return;

    if (frame->isWebLocalFrame())
        visitor->trace(toWebLocalFrameImpl(frame));
    else
        visitor->trace(toWebRemoteFrameImpl(frame));
}
