void XMLHttpRequest::didReceiveResponse(unsigned long identifier, const ResourceResponse& response)
{
    InspectorInstrumentation::didReceiveXHRResponse(scriptExecutionContext(), identifier);

    m_response = response;
    if (!m_mimeTypeOverride.isEmpty()) {
        m_response.setHTTPHeaderField("Content-Type", m_mimeTypeOverride);
        m_responseEncoding = extractCharsetFromMediaType(m_mimeTypeOverride);
    }

    if (m_responseEncoding.isEmpty())
        m_responseEncoding = response.textEncodingName();
}
