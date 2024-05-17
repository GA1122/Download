void WebPagePrivate::setDefaultLayoutSize(const IntSize& size)
{
    IntSize screenSize = Platform::Settings::instance()->applicationSize();
    ASSERT(size.width() <= screenSize.width() && size.height() <= screenSize.height());
    m_defaultLayoutSize = size.expandedTo(minimumLayoutSize).shrunkTo(screenSize);
}
