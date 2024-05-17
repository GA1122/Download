void WebDevToolsAgentImpl::dispatchMouseEvent(const PlatformMouseEvent& event)
{
    m_generatingEvent = true;
    WebMouseEvent webEvent = WebMouseEventBuilder(m_webViewImpl->mainFrameImpl()->frameView(), event);
    m_webViewImpl->handleInputEvent(webEvent);
    m_generatingEvent = false;
}
