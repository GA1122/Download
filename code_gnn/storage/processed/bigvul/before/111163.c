IntPoint WebPagePrivate::frameOffset(const Frame* frame) const
{
    ASSERT(frame);

    if (!mainFrame())
        return IntPoint();

    return mainFrame()->view()->windowToContents(frame->view()->contentsToWindow(IntPoint::zero()));
}
