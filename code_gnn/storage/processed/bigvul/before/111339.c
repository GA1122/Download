void WebPagePrivate::setCursor(PlatformCursor handle)
{
    if (m_currentCursor.type() != handle.type()) {
        m_currentCursor = handle;
        m_client->cursorChanged(handle.type(), handle.url().c_str(), handle.hotspot());
    }
}
