bool FrameView::isActive() const
{
    Page* page = frame().page();
    return page && page->focusController().isActive();
}
