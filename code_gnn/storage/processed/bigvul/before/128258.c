HostWindow* FrameView::hostWindow() const
{
    Page* page = frame().page();
    if (!page)
        return 0;
    return &page->chrome();
}
