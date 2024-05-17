void FrameView::init()
{
    reset();

    m_size = LayoutSize();

    Element* ownerElement = m_frame->deprecatedLocalOwner();
    if (ownerElement && (isHTMLFrameElement(*ownerElement) || isHTMLIFrameElement(*ownerElement))) {
        HTMLFrameElementBase* frameElt = toHTMLFrameElementBase(ownerElement);
        if (frameElt->scrollingMode() == ScrollbarAlwaysOff)
            setCanHaveScrollbars(false);
    }
}
