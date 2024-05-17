ALWAYS_INLINE bool WebFrame::isFrameAlive(const WebFrame* frame)
{
    if (!frame)
        return true;

    if (frame->isWebLocalFrame())
        return ThreadHeap::isHeapObjectAlive(toWebLocalFrameImpl(frame));

    return ThreadHeap::isHeapObjectAlive(toWebRemoteFrameImpl(frame));
}
