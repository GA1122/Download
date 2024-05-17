SiteInstance* WebContentsImpl::GetSiteInstance() const {
  return GetRenderManager()->current_host()->GetSiteInstance();
}
