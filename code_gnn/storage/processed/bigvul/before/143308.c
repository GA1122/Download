 Frame::Frame(FrameClient* client, FrameHost* host, FrameOwner* owner)
     : m_treeNode(this)
     , m_host(host)
    , m_owner(owner)
    , m_client(client)
    , m_isLoading(false)
{
    InstanceCounters::incrementCounter(InstanceCounters::FrameCounter);

    ASSERT(page());

    if (m_owner)
        m_owner->setContentFrame(*this);
    else
        page()->setMainFrame(this);
}
