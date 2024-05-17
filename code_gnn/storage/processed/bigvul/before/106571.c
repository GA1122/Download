static bool isDisconnectedFrame(WebFrameProxy* frame)
{
    return !frame->page() || !frame->page()->mainFrame() || !frame->isDescendantOf(frame->page()->mainFrame());
}
