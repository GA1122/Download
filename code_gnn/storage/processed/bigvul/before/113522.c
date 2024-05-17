void WebPagePrivate::updateCursor()
{
    int buttonMask = 0;
    if (m_lastMouseEvent.button() == LeftButton)
        buttonMask = Platform::MouseEvent::ScreenLeftMouseButton;
    else if (m_lastMouseEvent.button() == MiddleButton)
        buttonMask = Platform::MouseEvent::ScreenMiddleMouseButton;
    else if (m_lastMouseEvent.button() == RightButton)
        buttonMask = Platform::MouseEvent::ScreenRightMouseButton;

    unsigned modifiers = m_lastMouseEvent.shiftKey() ? 0 : KEYMOD_SHIFT |
        m_lastMouseEvent.ctrlKey() ? 0 : KEYMOD_CTRL |
        m_lastMouseEvent.altKey() ? 0 : KEYMOD_ALT;

    BlackBerry::Platform::MouseEvent event(buttonMask, buttonMask, mapToTransformed(m_lastMouseEvent.position()), mapToTransformed(m_lastMouseEvent.globalPosition()), 0, modifiers,  0);

    event.populateDocumentPosition(m_lastMouseEvent.position(), mapFromTransformedViewportToTransformedContents(m_lastMouseEvent.position()));
    m_webPage->mouseEvent(event);
}
