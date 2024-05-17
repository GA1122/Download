bool WebPage::performDefaultBehaviorForKeyEvent(const WebKeyboardEvent& keyboardEvent)
{
    if (keyboardEvent.type() != WebEvent::KeyDown && keyboardEvent.type() != WebEvent::RawKeyDown)
        return false;

    switch (keyboardEvent.windowsVirtualKeyCode()) {
    case VK_BACK:
        if (keyboardEvent.isSystemKey())
            return false;
        if (keyboardEvent.shiftKey())
            m_page->goForward();
        else
            m_page->goBack();
        break;
    case VK_LEFT:
        if (keyboardEvent.isSystemKey())
            m_page->goBack();
        else
            scroll(m_page.get(), ScrollLeft, ScrollByLine);
        break;
    case VK_RIGHT:
        if (keyboardEvent.isSystemKey())
            m_page->goForward();
        else
            scroll(m_page.get(), ScrollRight, ScrollByLine);
        break;
    case VK_UP:
        if (keyboardEvent.isSystemKey())
            return false;
        scroll(m_page.get(), ScrollUp, ScrollByLine);
        break;
    case VK_DOWN:
        if (keyboardEvent.isSystemKey())
            return false;
        scroll(m_page.get(), ScrollDown, ScrollByLine);
        break;
    case VK_HOME:
        if (keyboardEvent.isSystemKey())
            return false;
        logicalScroll(m_page.get(), ScrollBlockDirectionBackward, ScrollByDocument);
        break;
    case VK_END:
        if (keyboardEvent.isSystemKey())
            return false;
        logicalScroll(m_page.get(), ScrollBlockDirectionForward, ScrollByDocument);
        break;
    case VK_PRIOR:
        if (keyboardEvent.isSystemKey())
            return false;
        logicalScroll(m_page.get(), ScrollBlockDirectionBackward, ScrollByPage);
        break;
    case VK_NEXT:
        if (keyboardEvent.isSystemKey())
            return false;
        logicalScroll(m_page.get(), ScrollBlockDirectionForward, ScrollByPage);
        break;
    default:
        return false;
    }

    return true;
}
