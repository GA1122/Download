bool XMLHttpRequest::areMethodAndURLValidForSend()
{
    return m_method != "GET" && m_method != "HEAD" && m_url.protocolIsInHTTPFamily();
}
