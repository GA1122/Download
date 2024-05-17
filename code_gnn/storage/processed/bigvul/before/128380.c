IntRect FrameView::windowResizerRect() const
{
    Page* page = frame().page();
    if (!page)
        return IntRect();
    return page->chrome().windowResizerRect();
}
