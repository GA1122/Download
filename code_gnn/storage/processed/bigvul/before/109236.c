bool InspectorOverlay::handleMouseEvent(const PlatformMouseEvent& event)
{
    if (isEmpty())
        return false;

    EventHandler& eventHandler = overlayPage()->mainFrame()->eventHandler();
    bool result;
    switch (event.type()) {
    case PlatformEvent::MouseMoved:
        result = eventHandler.handleMouseMoveEvent(event);
        break;
    case PlatformEvent::MousePressed:
        result = eventHandler.handleMousePressEvent(event);
        break;
    case PlatformEvent::MouseReleased:
        result = eventHandler.handleMouseReleaseEvent(event);
        break;
    default:
        return false;
    }

    overlayPage()->mainFrame()->document()->updateLayout();
    return result;
}
