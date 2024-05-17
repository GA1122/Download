void WebPageProxy::handleWheelEvent(const WebWheelEvent& event)
{
    if (!isValid())
        return;

    if (m_processingWheelEvent) {
        m_nextWheelEvent = coalesceWheelEvents(m_nextWheelEvent.get(), event);
        return;
    }

    process()->responsivenessTimer()->start();
    process()->send(Messages::WebPage::WheelEvent(event), m_pageID);
    m_processingWheelEvent = true;
}
