bool WebPage::touchEvent(const Platform::TouchEvent& event)
{
#if DEBUG_TOUCH_EVENTS
    Platform::logAlways(Platform::LogLevelCritical, "%s", event.toString().c_str());
#endif

#if ENABLE(TOUCH_EVENTS)
    if (!d->m_mainFrame)
        return false;

    if (d->m_page->defersLoading())
        return false;

    if (d->m_inputHandler)
        d->m_inputHandler->setInputModeEnabled();

    PluginView* pluginView = d->m_fullScreenPluginView.get();
    if (pluginView)
        return d->dispatchTouchEventToFullScreenPlugin(pluginView, event);

    Platform::TouchEvent tEvent = event;
    if (event.isSingleTap())
        d->m_pluginMayOpenNewTab = true;
    else if (tEvent.m_type == Platform::TouchEvent::TouchStart || tEvent.m_type == Platform::TouchEvent::TouchCancel)
        d->m_pluginMayOpenNewTab = false;

    if (tEvent.m_type == Platform::TouchEvent::TouchStart) {
        d->clearCachedHitTestResult();
        d->m_touchEventHandler->doFatFingers(tEvent.m_points[0]);

        Element* elementUnderFatFinger = d->m_touchEventHandler->lastFatFingersResult().nodeAsElementIfApplicable();
        if (elementUnderFatFinger)
            d->m_touchEventHandler->drawTapHighlight();
    }

    bool handled = false;

    if (event.m_type != Platform::TouchEvent::TouchInjected)
        handled = d->m_mainFrame->eventHandler()->handleTouchEvent(PlatformTouchEvent(&tEvent));

    if (d->m_preventDefaultOnTouchStart) {
        if (tEvent.m_type == Platform::TouchEvent::TouchEnd || tEvent.m_type == Platform::TouchEvent::TouchCancel)
            d->m_preventDefaultOnTouchStart = false;
        return true;
    }

    if (handled) {
        if (tEvent.m_type == Platform::TouchEvent::TouchStart)
            d->m_preventDefaultOnTouchStart = true;
        return true;
    }
#endif

    return false;
}