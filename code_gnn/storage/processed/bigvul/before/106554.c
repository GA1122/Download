void WebPageProxy::handleMouseEvent(const WebMouseEvent& event)
{
    if (!isValid())
        return;

    if (event.type() != WebEvent::MouseMove)
        process()->responsivenessTimer()->start();
    else {
        if (m_processingMouseMoveEvent) {
            m_nextMouseMoveEvent = adoptPtr(new WebMouseEvent(event));
            return;
        }

        m_processingMouseMoveEvent = true;
    }

    process()->send(Messages::WebPage::MouseEvent(event), m_pageID);
}
