void RenderFrameHostImpl::SetLastCommittedSiteUrl(const GURL& url) {
  GURL site_url =
      url.is_empty() ? GURL()
                     : SiteInstance::GetSiteForURL(frame_tree_node_->navigator()
                                                       ->GetController()
                                                       ->GetBrowserContext(),
                                                   url);

  if (last_committed_site_url_ == site_url)
    return;

  if (!last_committed_site_url_.is_empty()) {
    RenderProcessHostImpl::RemoveFrameWithSite(
        frame_tree_node_->navigator()->GetController()->GetBrowserContext(),
        GetProcess(), last_committed_site_url_);
  }

  last_committed_site_url_ = site_url;

  if (!last_committed_site_url_.is_empty()) {
    RenderProcessHostImpl::AddFrameWithSite(
        frame_tree_node_->navigator()->GetController()->GetBrowserContext(),
        GetProcess(), last_committed_site_url_);
  }
}
