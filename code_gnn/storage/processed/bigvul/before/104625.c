bool Extension::HasHostPermission(const GURL& url) const {
  if (url.SchemeIs(chrome::kChromeUIScheme) &&
      url.host() != chrome::kChromeUIFaviconHost &&
      location() != Extension::COMPONENT)
    return false;
  return permission_set()->HasExplicitAccessToOrigin(url);
}
