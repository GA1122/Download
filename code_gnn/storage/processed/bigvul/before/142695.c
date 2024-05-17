static NavigationPolicy navigationPolicyForRequest(const FrameLoadRequest& request)
{
    NavigationPolicy policy = NavigationPolicyCurrentTab;
    Event* event = request.triggeringEvent();
    if (!event)
        return policy;

    if (request.form() && event->underlyingEvent())
        event = event->underlyingEvent();

    if (event->isMouseEvent()) {
        MouseEvent* mouseEvent = toMouseEvent(event);
        navigationPolicyFromMouseEvent(mouseEvent->button(), mouseEvent->ctrlKey(), mouseEvent->shiftKey(), mouseEvent->altKey(), mouseEvent->metaKey(), &policy);
    } else if (event->isKeyboardEvent()) {
        KeyboardEvent* keyEvent = toKeyboardEvent(event);
        navigationPolicyFromMouseEvent(0, keyEvent->ctrlKey(), keyEvent->shiftKey(), keyEvent->altKey(), keyEvent->metaKey(), &policy);
    } else if (event->isGestureEvent()) {
        GestureEvent* gestureEvent = toGestureEvent(event);
        navigationPolicyFromMouseEvent(0, gestureEvent->ctrlKey(), gestureEvent->shiftKey(), gestureEvent->altKey(), gestureEvent->metaKey(), &policy);
    }
    return policy;
}
