void FrameView::setCursor(const Cursor& cursor)
{
    Page* page = frame().page();
    if (!page || !page->settings().deviceSupportsMouse())
        return;
    page->chrome().setCursor(cursor);
}
