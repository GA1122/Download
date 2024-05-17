void WebPageProxy::didChangeContentsSize(const WebCore::IntSize& size)
{
    m_pageClient->didChangeContentsSize(size);
}
