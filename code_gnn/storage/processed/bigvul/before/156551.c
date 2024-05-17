bool ChildProcessSecurityPolicyImpl::IsIsolatedOrigin(
    const url::Origin& origin) {
  url::Origin unused_result;
  return GetMatchingIsolatedOrigin(origin, &unused_result);
}
