static WebKitWebNavigationAction* getNavigationAction(const NavigationAction& action, const char* targetFrame)
{
    gint button = -1;

    const Event* event = action.event();
    if (event && event->isMouseEvent()) {
        const MouseEvent* mouseEvent = static_cast<const MouseEvent*>(event);
        button = mouseEvent->button() + 1;
    }

    gint modifierFlags = 0;
    UIEventWithKeyState* keyStateEvent = findEventWithKeyState(const_cast<Event*>(event));
    if (keyStateEvent) {
        if (keyStateEvent->shiftKey())
            modifierFlags |= GDK_SHIFT_MASK;
        if (keyStateEvent->ctrlKey())
            modifierFlags |= GDK_CONTROL_MASK;
        if (keyStateEvent->altKey())
            modifierFlags |= GDK_MOD1_MASK;
        if (keyStateEvent->metaKey())
            modifierFlags |= GDK_MOD2_MASK;
    }

    return WEBKIT_WEB_NAVIGATION_ACTION(g_object_new(WEBKIT_TYPE_WEB_NAVIGATION_ACTION,
                                                     "reason", kit(action.type()),
                                                     "original-uri", action.url().string().utf8().data(),
                                                     "button", button,
                                                     "modifier-state", modifierFlags,
                                                     "target-frame", targetFrame,
                                                     NULL));
}
