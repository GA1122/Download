bool XSSAuditor::isSafeToSendToAnotherThread() const
{
    return m_documentURL.isSafeToSendToAnotherThread()
        && m_decodedURL.isSafeToSendToAnotherThread()
        && m_decodedHTTPBody.isSafeToSendToAnotherThread();
}
