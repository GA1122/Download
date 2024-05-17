bool CSPSource::hostMatches(const KURL& url) const
{
    const String& host = url.host();
    Document* document = m_policy->document();
    bool match;

    bool equalHosts = equalIgnoringCase(host, m_host);
    if (m_hostWildcard == HasWildcard) {
        match = host.endsWith("." + m_host, TextCaseInsensitive);

        if (document && equalHosts)
            UseCounter::count(*document, UseCounter::CSPSourceWildcardWouldMatchExactHost);
    } else {
        match = equalHosts;
    }

    return match;
}
