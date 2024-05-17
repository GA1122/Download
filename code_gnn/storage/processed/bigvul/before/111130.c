bool WebPagePrivate::dispatchMouseEventToFullScreenPlugin(PluginView* plugin, const Platform::MouseEvent& event)
{
    NPEvent npEvent;
    NPMouseEvent mouseEvent;

    mouseEvent.x = event.screenPosition().x();
    mouseEvent.y = event.screenPosition().y();

    switch (event.type()) {
    case Platform::MouseEvent::MouseButtonDown:
        mouseEvent.type = MOUSE_BUTTON_DOWN;
        m_pluginMouseButtonPressed = true;
        break;
    case Platform::MouseEvent::MouseButtonUp:
        mouseEvent.type = MOUSE_BUTTON_UP;
        m_pluginMouseButtonPressed = false;
        break;
    case Platform::MouseEvent::MouseMove:
        mouseEvent.type = MOUSE_MOTION;
        break;
    default:
        return false;
    }

    mouseEvent.flags = 0;
    mouseEvent.button = m_pluginMouseButtonPressed;

    npEvent.type = NP_MouseEvent;
    npEvent.data = &mouseEvent;

    return plugin->dispatchFullScreenNPEvent(npEvent);
}
