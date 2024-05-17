WebFrame::WebFrame()
    : m_coreFrame(0)
    , m_policyListenerID(0)
    , m_policyFunction(0)
    , m_policyDownloadID(0)
    , m_frameLoaderClient(this)
    , m_loadListener(0)
    , m_frameID(generateFrameID())
{
    WebProcess::shared().addWebFrame(m_frameID, this);

#ifndef NDEBUG
    webFrameCounter.increment();
#endif
}
