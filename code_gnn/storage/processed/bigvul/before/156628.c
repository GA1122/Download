void NavigatorImpl::RequestOpenURL(
    RenderFrameHostImpl* render_frame_host,
    const GURL& url,
    bool uses_post,
    const scoped_refptr<network::ResourceRequestBody>& body,
    const std::string& extra_headers,
    const Referrer& referrer,
    WindowOpenDisposition disposition,
    bool should_replace_current_entry,
    bool user_gesture,
    blink::WebTriggeringEventInfo triggering_event_info,
    scoped_refptr<network::SharedURLLoaderFactory> blob_url_loader_factory) {

  if (render_frame_host !=
      render_frame_host->frame_tree_node()->current_frame_host()) {
    return;
  }

  SiteInstance* current_site_instance = render_frame_host->GetSiteInstance();

  std::vector<GURL> redirect_chain;

  GURL dest_url(url);
  if (!GetContentClient()->browser()->ShouldAllowOpenURL(current_site_instance,
                                                         url)) {
    dest_url = GURL(url::kAboutBlankURL);
  }

  int frame_tree_node_id = -1;

  if (disposition == WindowOpenDisposition::CURRENT_TAB &&
      render_frame_host->GetParent()) {
    frame_tree_node_id =
        render_frame_host->frame_tree_node()->frame_tree_node_id();
  }

  OpenURLParams params(dest_url, referrer, frame_tree_node_id, disposition,
                       ui::PAGE_TRANSITION_LINK,
                       true  );
  params.uses_post = uses_post;
  params.post_data = body;
  params.extra_headers = extra_headers;
  if (redirect_chain.size() > 0)
    params.redirect_chain = redirect_chain;
  params.should_replace_current_entry = should_replace_current_entry;
  params.user_gesture = user_gesture;
  params.triggering_event_info = triggering_event_info;

  params.source_site_instance = current_site_instance;

  params.source_render_frame_id = render_frame_host->GetRoutingID();
  params.source_render_process_id = render_frame_host->GetProcess()->GetID();

  if (render_frame_host->web_ui()) {
    params.referrer = Referrer();

    params.is_renderer_initiated = false;
  }

  params.blob_url_loader_factory = std::move(blob_url_loader_factory);

  GetContentClient()->browser()->OverrideNavigationParams(
      current_site_instance, &params.transition, &params.is_renderer_initiated,
      &params.referrer);

  if (delegate_)
    delegate_->OpenURL(params);
}
