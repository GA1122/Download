WebFrame* WebFrame::fromFrame(Frame* frame)
{
    if (!frame)
        return 0;

    if (frame->isLocalFrame())
        return WebLocalFrameImpl::fromFrame(toLocalFrame(*frame));
    return WebRemoteFrameImpl::fromFrame(toRemoteFrame(*frame));
}
