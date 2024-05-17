void FrameView::setContentsSize(const IntSize& size)
{
    if (size == contentsSize())
        return;

    ScrollView::setContentsSize(size);
    ScrollView::contentsResized();

    Page* page = frame().page();
    if (!page)
        return;

    updateScrollableAreaSet();

    page->chrome().contentsSizeChanged(m_frame.get(), size);
}
