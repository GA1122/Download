bool PopupContainer::handleMouseReleaseEvent(const PlatformMouseEvent& event)
{
    RefPtr<PopupContainer> protect(this);
    UserGestureIndicator gestureIndicator(DefinitelyProcessingUserGesture);
    return m_listBox->handleMouseReleaseEvent(
        constructRelativeMouseEvent(event, this, m_listBox.get()));
}
