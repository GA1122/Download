bool FrameView::isScrollable()
{

    IntSize contentsSize = this->contentsSize();
    IntSize visibleContentSize = visibleContentRect().size();
    if ((contentsSize.height() <= visibleContentSize.height() && contentsSize.width() <= visibleContentSize.width()))
        return false;

    HTMLFrameOwnerElement* owner = m_frame->deprecatedLocalOwner();
    if (owner && (!owner->renderer() || !owner->renderer()->visibleToHitTesting()))
        return false;

    ScrollbarMode horizontalMode;
    ScrollbarMode verticalMode;
    calculateScrollbarModesForLayoutAndSetViewportRenderer(horizontalMode, verticalMode, RulesFromWebContentOnly);
    if (horizontalMode == ScrollbarAlwaysOff && verticalMode == ScrollbarAlwaysOff)
        return false;

    return true;
}
