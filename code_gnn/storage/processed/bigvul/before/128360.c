void FrameView::updateBackgroundRecursively(const Color& backgroundColor, bool transparent)
{
    for (Frame* frame = m_frame.get(); frame; frame = frame->tree().traverseNext(m_frame.get())) {
        if (!frame->isLocalFrame())
            continue;
        if (FrameView* view = toLocalFrame(frame)->view()) {
            view->setTransparent(transparent);
            view->setBaseBackgroundColor(backgroundColor);
        }
    }
}
