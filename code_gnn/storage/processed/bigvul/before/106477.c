void WebPageProxy::didCreateMainFrame(uint64_t frameID)
{
    MESSAGE_CHECK(!m_mainFrame);
    MESSAGE_CHECK(process()->canCreateFrame(frameID));

    m_mainFrame = WebFrameProxy::create(this, frameID);

    process()->frameCreated(frameID, m_mainFrame.get());
}
