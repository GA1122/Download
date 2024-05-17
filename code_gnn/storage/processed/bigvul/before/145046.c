bool CSPSource::portMatches(const KURL& url) const
{
    if (m_portWildcard == HasWildcard)
        return true;

    int port = url.port();

    if (port == m_port)
        return true;

    if (!port)
        return isDefaultPortForProtocol(m_port, url.protocol());

    if (!m_port)
        return isDefaultPortForProtocol(port, url.protocol());

    return false;
}
