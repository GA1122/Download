bool ChildProcessSecurityPolicyImpl::CanRedirectToURL(const GURL& url) {
  if (!url.is_valid())
    return false;   

  const std::string& scheme = url.scheme();

  if (scheme == kChromeErrorScheme)
    return false;

  if (IsPseudoScheme(scheme)) {
    return url.IsAboutBlank();
  }


  return true;
 }
