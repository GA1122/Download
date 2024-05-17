void WebDevToolsAgentImpl::dispatchKeyEvent(const PlatformKeyboardEvent& event)
{
    m_generatingEvent = true;
    WebKeyboardEvent webEvent = WebKeyboardEventBuilder(event);
    if (!webEvent.keyIdentifier[0] && webEvent.type != WebInputEvent::Char)
        webEvent.setKeyIdentifierFromWindowsKeyCode();
    m_webViewImpl->handleInputEvent(webEvent);
    m_generatingEvent = false;
}
