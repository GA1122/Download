FrameView* SVGImage::frameView() const
{
    if (!m_page)
        return 0;

    return m_page->mainFrame()->view();
}
