bool InspectorOverlay::handleGestureEvent(const PlatformGestureEvent& event)
{
    if (isEmpty())
        return false;

    return overlayPage()->mainFrame()->eventHandler().handleGestureEvent(event);
}
