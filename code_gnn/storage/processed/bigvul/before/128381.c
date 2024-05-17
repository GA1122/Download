FrameView::~FrameView()
{
    if (m_postLayoutTasksTimer.isActive())
        m_postLayoutTasksTimer.stop();

    if (m_didScrollTimer.isActive())
        m_didScrollTimer.stop();

    removeFromAXObjectCache();
    resetScrollbars();

    ASSERT(!horizontalScrollbar() || !horizontalScrollbar()->isCustomScrollbar());
    ASSERT(!verticalScrollbar() || !verticalScrollbar()->isCustomScrollbar());

    setHasHorizontalScrollbar(false);  
    setHasVerticalScrollbar(false);

    ASSERT(!m_scrollCorner);

    ASSERT(m_frame);
    ASSERT(m_frame->view() != this || !m_frame->contentRenderer());
    HTMLFrameOwnerElement* ownerElement = m_frame->deprecatedLocalOwner();
    if (ownerElement && ownerElement->ownedWidget() == this)
        ownerElement->setWidget(nullptr);
}
