void WebPageProxy::windowToScreen(const IntRect& viewRect, IntRect& result)
{
    result = m_pageClient->windowToScreen(viewRect);
}
