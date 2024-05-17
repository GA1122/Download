void WebPageProxy::scrollBy(ScrollDirection direction, ScrollGranularity granularity)
{
    if (!isValid())
        return;

    process()->send(Messages::WebPage::ScrollBy(direction, granularity), m_pageID);
}
