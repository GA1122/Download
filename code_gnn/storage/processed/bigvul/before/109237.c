bool InspectorOverlay::handleTouchEvent(const PlatformTouchEvent& event)
{
    if (isEmpty())
        return false;

    return overlayPage()->mainFrame()->eventHandler().handleTouchEvent(event);
}
