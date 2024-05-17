bool RenderFrameHostManager::IsCurrentlySameSite(RenderFrameHostImpl* candidate,
                                                 const GURL& dest_url) {
  BrowserContext* browser_context =
      delegate_->GetControllerForRenderManager().GetBrowserContext();
  bool src_has_effective_url = SiteInstanceImpl::HasEffectiveURL(
      browser_context, candidate->GetSiteInstance()->original_url());
  bool dest_has_effective_url =
      SiteInstanceImpl::HasEffectiveURL(browser_context, dest_url);
  bool should_compare_effective_urls = true;
  if (!frame_tree_node_->IsMainFrame() ||
      (src_has_effective_url && !dest_has_effective_url &&
       candidate->GetSiteInstance()->GetRelatedActiveContentsCount() > 1u))
    should_compare_effective_urls = false;

  bool should_check_for_wrong_process =
      should_compare_effective_urls ||
      (!src_has_effective_url && !dest_has_effective_url);
  if (should_check_for_wrong_process &&
      candidate->GetSiteInstance()->HasWrongProcessForURL(dest_url))
    return false;

  if (candidate->last_successful_url().is_empty()) {
    return SiteInstanceImpl::IsSameWebSite(
        browser_context, candidate->GetSiteInstance()->original_url(), dest_url,
        should_compare_effective_urls);
  }

  if (SiteInstanceImpl::IsSameWebSite(
          browser_context, candidate->last_successful_url(), dest_url,
          should_compare_effective_urls)) {
    return true;
  }

  if (!candidate->GetLastCommittedOrigin().unique() &&
      SiteInstanceImpl::IsSameWebSite(
          browser_context,
          GURL(candidate->GetLastCommittedOrigin().Serialize()), dest_url,
          should_compare_effective_urls)) {
    return true;
  }

  if (candidate->last_successful_url().IsAboutBlank() &&
      candidate->GetLastCommittedOrigin().unique() &&
      SiteInstanceImpl::IsSameWebSite(
          browser_context, candidate->GetSiteInstance()->original_url(),
          dest_url, should_compare_effective_urls)) {
    return true;
  }

  return false;
}
