void WebPagePrivate::setDefaultLayoutSize(const IntSize& size)
{
    IntSize screenSize = Platform::Graphics::Screen::primaryScreen()->size();
    ASSERT(size.width() <= screenSize.width() && size.height() <= screenSize.height());
    m_defaultLayoutSize = size.expandedTo(minimumLayoutSize).shrunkTo(screenSize);
}
