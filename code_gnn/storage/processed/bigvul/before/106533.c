WebFullScreenManagerProxy* WebPageProxy::fullScreenManager()
{
    if (!m_fullScreenManager)
        m_fullScreenManager = WebFullScreenManagerProxy::create(this);
    return m_fullScreenManager.get();
}
