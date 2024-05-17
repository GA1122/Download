void RenderLayerScrollableArea::updateScrollableAreaSet(bool hasOverflow)
{
    LocalFrame* frame = box().frame();
    if (!frame)
        return;

    FrameView* frameView = frame->view();
    if (!frameView)
        return;

    bool isVisibleToHitTest = box().visibleToHitTesting();
    if (HTMLFrameOwnerElement* owner = frame->deprecatedLocalOwner())
        isVisibleToHitTest &= owner->renderer() && owner->renderer()->visibleToHitTesting();

    bool didScrollOverflow = m_scrollsOverflow;

    m_scrollsOverflow = hasOverflow && isVisibleToHitTest;
    if (didScrollOverflow == scrollsOverflow())
        return;

    if (m_scrollsOverflow)
        frameView->addScrollableArea(this);
    else
        frameView->removeScrollableArea(this);
}
