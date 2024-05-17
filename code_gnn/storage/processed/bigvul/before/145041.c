CSPSource::CSPSource(ContentSecurityPolicy* policy, const String& scheme, const String& host, int port, const String& path, WildcardDisposition hostWildcard, WildcardDisposition portWildcard)
    : m_policy(policy)
    , m_scheme(scheme)
    , m_host(host)
    , m_port(port)
    , m_path(path)
    , m_hostWildcard(hostWildcard)
    , m_portWildcard(portWildcard)
{
}
