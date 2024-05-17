void WebPageProxy::handleGestureEvent(const WebGestureEvent& event)
{
    if (!isValid())
        return;

    process()->responsivenessTimer()->start();
    process()->send(Messages::WebPage::GestureEvent(event), m_pageID);
}
