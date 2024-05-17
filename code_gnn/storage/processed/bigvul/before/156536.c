bool ChildProcessSecurityPolicyImpl::GetMatchingIsolatedOrigin(
    const url::Origin& origin,
    url::Origin* result) {
  return GetMatchingIsolatedOrigin(
      origin, SiteInstanceImpl::GetSiteForOrigin(origin), result);
}
