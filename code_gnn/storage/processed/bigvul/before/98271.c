FrameLoaderClient::FrameLoaderClient(WebKitWebFrame* frame)
    : m_frame(frame)
    , m_policyDecision(0)
    , m_pluginView(0)
    , m_hasSentResponseToPlugin(false)
{
    ASSERT(m_frame);
}
