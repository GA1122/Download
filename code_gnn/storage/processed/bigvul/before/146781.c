bool Document::IsSecureTransitionTo(const KURL& url) const {
  RefPtr<SecurityOrigin> other = SecurityOrigin::Create(url);
  return GetSecurityOrigin()->CanAccess(other.Get());
}
