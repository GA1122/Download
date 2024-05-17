void WebFrame::invalidate()
{
    WebProcess::shared().removeWebFrame(m_frameID);
    m_coreFrame = 0;
}
