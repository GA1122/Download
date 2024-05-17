void WebPageProxy::pageDidRequestScroll(const IntPoint& point)
{
    m_pageClient->pageDidRequestScroll(point);
}
