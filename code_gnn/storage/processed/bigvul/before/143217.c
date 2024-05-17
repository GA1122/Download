bool Document::isSecureTransitionTo(const KURL& url) const
{
    RefPtr<SecurityOrigin> other = SecurityOrigin::create(url);
    return getSecurityOrigin()->canAccess(other.get());
}
