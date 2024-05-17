bool ChildProcessSecurityPolicyImpl::CanRequestURL(
    int child_id, const GURL& url) {
  if (!url.is_valid())
    return false;   

  if (IsPseudoScheme(url.scheme())) {

    if (url.SchemeIs(kViewSourceScheme)) {
      GURL child_url(url.GetContent());
      if (child_url.SchemeIs(kViewSourceScheme) &&
          url.SchemeIs(kViewSourceScheme))
          return false;

      return CanRequestURL(child_id, child_url);
    }

    if (base::LowerCaseEqualsASCII(url.spec(), url::kAboutBlankURL))
      return true;   

    return false;
  }

  if (CanCommitURL(child_id, url))
    return true;

  return !GetContentClient()->browser()->IsHandledURL(url) &&
         !net::URLRequest::IsHandledURL(url);
}
