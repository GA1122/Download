void WebPageProxy::runOpenPanel(uint64_t frameID, const WebOpenPanelParameters::Data& data)
{
    if (m_openPanelResultListener) {
        m_openPanelResultListener->invalidate();
        m_openPanelResultListener = 0;
    }

    WebFrameProxy* frame = process()->webFrame(frameID);
    MESSAGE_CHECK(frame);

    m_openPanelResultListener = WebOpenPanelResultListenerProxy::create(this);

    if (!m_uiClient.runOpenPanel(this, frame, data, m_openPanelResultListener.get()))
        didCancelForOpenPanel();
}
