void WebPagePrivate::didReceiveTouchEventMode(TouchEventMode mode)
{
    if (mode != m_touchEventMode)
        m_client->touchEventModeChanged(toPlatformTouchEventMode(mode));
    m_touchEventMode = mode;
}
