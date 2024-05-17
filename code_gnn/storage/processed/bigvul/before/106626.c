void WebPageProxy::setFixedLayoutSize(const IntSize& size)
{
    if (!isValid())
        return;

    if (size == m_fixedLayoutSize)
        return;

    m_fixedLayoutSize = size;
    process()->send(Messages::WebPage::SetFixedLayoutSize(size), m_pageID);
}
