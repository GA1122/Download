bool RenderFrameHostManager::IsRendererTransferNeededForNavigation(
    RenderFrameHostImpl* rfh,
    const GURL& dest_url) {
  if (!rfh->GetSiteInstance()->HasSite())
    return rfh->GetSiteInstance()->HasWrongProcessForURL(dest_url);

  if (rfh->GetSiteInstance()->GetSiteURL().SchemeIs(kGuestScheme))
    return false;

  BrowserContext* context = rfh->GetSiteInstance()->GetBrowserContext();
  if (IsCurrentlySameSite(rfh, dest_url)) {
    return false;
  }

  if (rfh->GetSiteInstance()->RequiresDedicatedProcess() ||
      SiteInstanceImpl::DoesSiteRequireDedicatedProcess(context,
                                                        dest_url)) {
    return true;
  }

  if (SiteIsolationPolicy::IsTopDocumentIsolationEnabled() &&
      (!frame_tree_node_->IsMainFrame() ||
       rfh->GetSiteInstance()->IsDefaultSubframeSiteInstance())) {
    return true;
  }

  FrameTreeNode* opener = frame_tree_node_->opener();
  if (opener && IsCurrentlySameSite(opener->current_frame_host(), dest_url) &&
      opener->current_frame_host()->GetSiteInstance() != rfh->GetSiteInstance())
    return true;

  return false;
}