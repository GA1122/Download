void WebPageProxy::dragEnded(const WebCore::IntPoint& clientPosition, const WebCore::IntPoint& globalPosition, uint64_t operation)
{
    if (!isValid())
        return;
    process()->send(Messages::WebPage::DragEnded(clientPosition, globalPosition, operation), m_pageID);
}
