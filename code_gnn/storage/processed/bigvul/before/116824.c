bool PopupContainer::handleKeyEvent(const PlatformKeyboardEvent& event)
{
    UserGestureIndicator gestureIndicator(DefinitelyProcessingUserGesture);
    return m_listBox->handleKeyEvent(event);
}
