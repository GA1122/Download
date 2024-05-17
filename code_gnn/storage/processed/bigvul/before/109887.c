bool SecurityContext::isSecureTransitionTo(const KURL& url) const
{
    if (!haveInitializedSecurityOrigin())
        return true;

    RefPtr<SecurityOrigin> other = SecurityOrigin::create(url);
    return securityOrigin()->canAccess(other.get());
}
