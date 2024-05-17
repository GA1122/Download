void FrameView::scrollContentsIfNeededRecursive()
{
    scrollContentsIfNeeded();

    for (Frame* child = m_frame->tree().firstChild(); child; child = child->tree().nextSibling()) {
        if (!child->isLocalFrame())
            continue;
        if (FrameView* view = toLocalFrame(child)->view())
            view->scrollContentsIfNeededRecursive();
    }
}
