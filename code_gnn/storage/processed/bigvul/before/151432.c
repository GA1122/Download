KURL FrameFetchContext::GetSiteForCookies() const {
  if (IsDetached())
    return frozen_state_->site_for_cookies;

  Document* document = document_ ? document_.Get() : GetFrame()->GetDocument();
  return document->SiteForCookies();
}
