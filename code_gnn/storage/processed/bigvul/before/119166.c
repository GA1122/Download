 void XMLHttpRequest::didFinishLoading(unsigned long identifier, double)
{
    if (m_error)
        return;

    if (m_state < HEADERS_RECEIVED)
        changeState(HEADERS_RECEIVED);

    if (m_decoder)
        m_responseText = m_responseText.concatenateWith(m_decoder->flush());

    if (m_responseStream)
        m_responseStream->finalize();

    InspectorInstrumentation::didFinishXHRLoading(scriptExecutionContext(), this, identifier, m_responseText, m_url, m_lastSendURL, m_lastSendLineNumber);

    RefPtr<XMLHttpRequest> protect(this);

    if (m_loader) {
        m_loader = 0;
        dropProtection();
    }

    changeState(DONE);
    m_responseEncoding = String();
    m_decoder = 0;
}
