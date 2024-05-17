void WebRTCVoidRequest::requestFailed(const WebString& error) const
{
    ASSERT(m_private.get());
    m_private->requestFailed(error);
}
