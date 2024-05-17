bool ContentSecurityPolicy::shouldBypassContentSecurityPolicy(
    const KURL& url,
    SchemeRegistry::PolicyAreas area) {
  if (SecurityOrigin::shouldUseInnerURL(url)) {
    return SchemeRegistry::schemeShouldBypassContentSecurityPolicy(
        SecurityOrigin::extractInnerURL(url).protocol(), area);
  } else {
    return SchemeRegistry::schemeShouldBypassContentSecurityPolicy(
        url.protocol(), area);
  }
}
