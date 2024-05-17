void WebPagePrivate::didReceiveCursorEventMode(CursorEventMode mode)
{
    if (mode != m_cursorEventMode)
        m_client->cursorEventModeChanged(toPlatformCursorEventMode(mode));
    m_cursorEventMode = mode;
}
