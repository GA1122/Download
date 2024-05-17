FrameLoaderClient::~FrameLoaderClient()
{
    if (m_policyDecision)
        g_object_unref(m_policyDecision);
}
