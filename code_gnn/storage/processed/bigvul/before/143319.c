Frame* Frame::findFrameForNavigation(const AtomicString& name, Frame& activeFrame)
{
    Frame* frame = tree().find(name);
    if (!frame || !activeFrame.canNavigate(*frame))
        return nullptr;
    return frame;
}
