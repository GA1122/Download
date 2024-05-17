void WebPageProxy::setUseFixedLayout(bool fixed)
{
    if (!isValid())
        return;

    if (fixed == m_useFixedLayout)
        return;

    m_useFixedLayout = fixed;
    if (!fixed)
        m_fixedLayoutSize = IntSize();
    process()->send(Messages::WebPage::SetUseFixedLayout(fixed), m_pageID);
}
