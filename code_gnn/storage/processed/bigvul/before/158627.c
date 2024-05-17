bool FrameFetchContext::IsFirstPartyOrigin(const KURL& url) const {
  if (IsDetached())
    return false;

  return GetFrame()
      ->Tree()
      .Top()
      .GetSecurityContext()
      ->GetSecurityOrigin()
      ->IsSameSchemeHostPort(SecurityOrigin::Create(url).get());
}
