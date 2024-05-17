bool Document::IsSecureTransitionTo(const KURL& url) const {
  scoped_refptr<const SecurityOrigin> other = SecurityOrigin::Create(url);
  return GetSecurityOrigin()->CanAccess(other.get());
}
