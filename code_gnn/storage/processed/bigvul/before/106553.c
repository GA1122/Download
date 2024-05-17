void WebPageProxy::handleKeyboardEvent(const NativeWebKeyboardEvent& event)
{
    if (!isValid())
        return;

    m_keyEventQueue.append(event);

    process()->responsivenessTimer()->start();
    process()->send(Messages::WebPage::KeyEvent(event), m_pageID);
}
