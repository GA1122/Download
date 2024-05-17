static bool shouldInheritSecurityOriginFromOwner(const KURL& url)
{
    return url.isEmpty() || url.protocolIsAbout();
}
