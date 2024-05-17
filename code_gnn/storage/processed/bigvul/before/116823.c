bool PopupContainer::handleGestureEvent(const PlatformGestureEvent& gestureEvent)
{
    switch (gestureEvent.type()) {
    case PlatformEvent::GestureTap: {
        PlatformMouseEvent fakeMouseMove(gestureEvent.position(), gestureEvent.globalPosition(), NoButton, PlatformEvent::MouseMoved,   1, gestureEvent.shiftKey(), gestureEvent.ctrlKey(), gestureEvent.altKey(), gestureEvent.metaKey(), gestureEvent.timestamp());
        PlatformMouseEvent fakeMouseDown(gestureEvent.position(), gestureEvent.globalPosition(), LeftButton, PlatformEvent::MousePressed,   1, gestureEvent.shiftKey(), gestureEvent.ctrlKey(), gestureEvent.altKey(), gestureEvent.metaKey(), gestureEvent.timestamp());
        PlatformMouseEvent fakeMouseUp(gestureEvent.position(), gestureEvent.globalPosition(), LeftButton, PlatformEvent::MouseReleased,   1, gestureEvent.shiftKey(), gestureEvent.ctrlKey(), gestureEvent.altKey(), gestureEvent.metaKey(), gestureEvent.timestamp());
        handleMouseDownEvent(fakeMouseDown);
        handleMouseReleaseEvent(fakeMouseUp);
        return true;
    }
    case PlatformEvent::GestureDoubleTap:
        break;
    case PlatformEvent::GestureScrollUpdate: {
        PlatformWheelEvent syntheticWheelEvent(gestureEvent.position(), gestureEvent.globalPosition(), gestureEvent.deltaX(), gestureEvent.deltaY(), gestureEvent.deltaX() / 120.0f, gestureEvent.deltaY() / 120.0f, ScrollByPixelWheelEvent, gestureEvent.shiftKey(), gestureEvent.ctrlKey(), gestureEvent.altKey(), gestureEvent.metaKey());
        handleWheelEvent(syntheticWheelEvent);
        return true;
    }
    case PlatformEvent::GestureScrollBegin:
    case PlatformEvent::GestureScrollEnd:
    case PlatformEvent::GestureTapDown:
        break;
    default:
        ASSERT_NOT_REACHED();
    }
    return false;
}
