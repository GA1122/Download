void Frame::detach(FrameDetachType type)
{
    ASSERT(m_client);
    m_client->setOpener(0);
    domWindow()->resetLocation();
    disconnectOwnerElement();
    m_client->detached(type);
    m_client = nullptr;
    m_host = nullptr;
}
