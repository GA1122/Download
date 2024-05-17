bool WebPage::mouseEvent(const Platform::MouseEvent& mouseEvent, bool* wheelDeltaAccepted)
{
    if (!d->m_mainFrame->view())
        return false;

    if (d->m_page->defersLoading())
        return false;

    PluginView* pluginView = d->m_fullScreenPluginView.get();
    if (pluginView)
        return d->dispatchMouseEventToFullScreenPlugin(pluginView, mouseEvent);

    if (mouseEvent.type() == Platform::MouseEvent::MouseAborted) {
        d->m_mainFrame->eventHandler()->setMousePressed(false);
        return false;
    }

    d->m_pluginMayOpenNewTab = true;

    d->m_lastUserEventTimestamp = currentTime();
    int clickCount = (d->m_selectionHandler->isSelectionActive() || mouseEvent.type() != Platform::MouseEvent::MouseMove) ? 1 : 0;

    MouseButton buttonType = NoButton;
    if (mouseEvent.isLeftButton())
        buttonType = LeftButton;
    else if (mouseEvent.isRightButton())
        buttonType = RightButton;
    else if (mouseEvent.isMiddleButton())
        buttonType = MiddleButton;

    PlatformMouseEvent platformMouseEvent(mouseEvent.documentViewportPosition(), mouseEvent.screenPosition(),
        toWebCoreMouseEventType(mouseEvent.type()), clickCount, buttonType,
        mouseEvent.shiftActive(), mouseEvent.ctrlActive(), mouseEvent.altActive(), PointingDevice);
    d->m_lastMouseEvent = platformMouseEvent;
    bool success = d->handleMouseEvent(platformMouseEvent);

    if (mouseEvent.wheelTicks()) {
        PlatformWheelEvent wheelEvent(mouseEvent.documentViewportPosition(), mouseEvent.screenPosition(),
            0, -mouseEvent.wheelDelta(),
            0, -mouseEvent.wheelTicks(),
            ScrollByPixelWheelEvent,
            mouseEvent.shiftActive(), mouseEvent.ctrlActive(),
            mouseEvent.altActive(), false  );
        if (wheelDeltaAccepted)
            *wheelDeltaAccepted = d->handleWheelEvent(wheelEvent);
    } else if (wheelDeltaAccepted)
        *wheelDeltaAccepted = false;

    return success;
}
