void NavigatorImpl::NavigateFromFrameProxy(
    RenderFrameHostImpl* render_frame_host,
    const GURL& url,
    SiteInstance* source_site_instance,
    const Referrer& referrer,
    ui::PageTransition page_transition,
    bool should_replace_current_entry,
    const std::string& method,
    scoped_refptr<network::ResourceRequestBody> post_body,
    const std::string& extra_headers,
    scoped_refptr<network::SharedURLLoaderFactory> blob_url_loader_factory) {
  if (method != "POST" && post_body) {
    NOTREACHED();
    post_body = nullptr;
  }

  if (!delegate_->ShouldTransferNavigation(
          render_frame_host->frame_tree_node()->IsMainFrame()))
    return;

  Referrer referrer_to_use(referrer);
  SiteInstance* current_site_instance = render_frame_host->GetSiteInstance();
  if (!GetContentClient()->browser()->ShouldAllowOpenURL(
          source_site_instance ? source_site_instance : current_site_instance,
          url)) {
    return;
  }

  bool is_renderer_initiated = true;
  if (render_frame_host->web_ui()) {
    referrer_to_use = Referrer();

    is_renderer_initiated = false;
  }

  GetContentClient()->browser()->OverrideNavigationParams(
      current_site_instance, &page_transition, &is_renderer_initiated,
      &referrer_to_use);

  controller_->NavigateFromFrameProxy(
      render_frame_host, url, is_renderer_initiated, source_site_instance,
      referrer_to_use, page_transition, should_replace_current_entry, method,
      post_body, extra_headers, std::move(blob_url_loader_factory));
}
