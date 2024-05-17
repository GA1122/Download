void WebRTCVoidRequest::requestSucceeded() const
{
    ASSERT(m_private.get());
    m_private->requestSucceeded();
}
