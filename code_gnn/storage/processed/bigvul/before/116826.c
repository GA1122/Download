bool PopupContainer::handleMouseMoveEvent(const PlatformMouseEvent& event)
{
    UserGestureIndicator gestureIndicator(DefinitelyProcessingUserGesture);
    return m_listBox->handleMouseMoveEvent(
        constructRelativeMouseEvent(event, this, m_listBox.get()));
}
