bool CSPSource::matches(const KURL& url, ContentSecurityPolicy::RedirectStatus redirectStatus) const
{
    if (!schemeMatches(url))
        return false;
    if (isSchemeOnly())
        return true;
    bool pathsMatch = (redirectStatus == ContentSecurityPolicy::DidRedirect) || pathMatches(url);
    return hostMatches(url) && portMatches(url) && pathsMatch;
}
