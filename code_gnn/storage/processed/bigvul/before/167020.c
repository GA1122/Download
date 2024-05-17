void NavigationControllerImpl::NavigateFromFrameProxy(
    RenderFrameHostImpl* render_frame_host,
    const GURL& url,
    bool is_renderer_initiated,
    SiteInstance* source_site_instance,
    const Referrer& referrer,
    ui::PageTransition page_transition,
    bool should_replace_current_entry,
    const std::string& method,
    scoped_refptr<network::ResourceRequestBody> post_body,
    const std::string& extra_headers,
    scoped_refptr<network::SharedURLLoaderFactory> blob_url_loader_factory) {
  FrameTreeNode* node = render_frame_host->frame_tree_node();
  std::unique_ptr<NavigationEntryImpl> entry;
  if (!node->IsMainFrame()) {
    if (GetLastCommittedEntry()) {
      entry = GetLastCommittedEntry()->Clone();
      entry->set_extra_headers(extra_headers);
    } else {
      entry = NavigationEntryImpl::FromNavigationEntry(CreateNavigationEntry(
          GURL(url::kAboutBlankURL), referrer, page_transition,
          is_renderer_initiated, extra_headers, browser_context_,
          nullptr  ));
    }
    entry->AddOrUpdateFrameEntry(
        node, -1, -1, nullptr,
        static_cast<SiteInstanceImpl*>(source_site_instance), url, referrer,
        std::vector<GURL>(), PageState(), method, -1, blob_url_loader_factory);
  } else {
    entry = NavigationEntryImpl::FromNavigationEntry(CreateNavigationEntry(
        url, referrer, page_transition, is_renderer_initiated, extra_headers,
        browser_context_, blob_url_loader_factory));
    entry->root_node()->frame_entry->set_source_site_instance(
        static_cast<SiteInstanceImpl*>(source_site_instance));
    entry->root_node()->frame_entry->set_method(method);
  }

  if (should_replace_current_entry && GetEntryCount() > 0)
    entry->set_should_replace_entry(true);
  if (GetLastCommittedEntry() &&
      GetLastCommittedEntry()->GetIsOverridingUserAgent()) {
    entry->SetIsOverridingUserAgent(true);
  }

  scoped_refptr<FrameNavigationEntry> frame_entry(entry->GetFrameEntry(node));
  if (!frame_entry) {
    frame_entry = new FrameNavigationEntry(
        node->unique_name(), -1, -1, nullptr,
        static_cast<SiteInstanceImpl*>(source_site_instance), url, referrer,
        std::vector<GURL>(), PageState(), method, -1, blob_url_loader_factory);
  }

  std::unique_ptr<NavigationRequest> request = CreateNavigationRequest(
      render_frame_host->frame_tree_node(), *entry, frame_entry.get(),
      ReloadType::NONE, false  ,
      false  , post_body, nullptr);

  if (!request)
    return;

  render_frame_host->frame_tree_node()->navigator()->Navigate(
      std::move(request), ReloadType::NONE, RestoreType::NONE);
}
