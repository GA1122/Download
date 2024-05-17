void NavigatorImpl::RequestOpenURL(
    RenderFrameHostImpl* render_frame_host,
    const GURL& url,
    bool uses_post,
    const scoped_refptr<ResourceRequestBodyImpl>& body,
    const std::string& extra_headers,
    SiteInstance* source_site_instance,
    const Referrer& referrer,
    WindowOpenDisposition disposition,
    bool should_replace_current_entry,
    bool user_gesture) {

  if (render_frame_host !=
      render_frame_host->frame_tree_node()->current_frame_host()) {
    return;
  }

  SiteInstance* current_site_instance = render_frame_host->GetSiteInstance();

  std::vector<GURL> redirect_chain;

  GURL dest_url(url);
  if (!GetContentClient()->browser()->ShouldAllowOpenURL(
          current_site_instance, url)) {
    dest_url = GURL(url::kAboutBlankURL);
  }

  int frame_tree_node_id = -1;

  if (SiteIsolationPolicy::UseSubframeNavigationEntries() &&
      disposition == WindowOpenDisposition::CURRENT_TAB &&
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
  params.source_site_instance = source_site_instance;
  if (redirect_chain.size() > 0)
    params.redirect_chain = redirect_chain;
  params.should_replace_current_entry = should_replace_current_entry;
  params.user_gesture = user_gesture;

  if (render_frame_host->web_ui()) {
    if (ui::PageTransitionCoreTypeIs(
        params.transition, ui::PAGE_TRANSITION_LINK))
      params.transition = render_frame_host->web_ui()->GetLinkTransitionType();

    params.referrer = Referrer();

    params.is_renderer_initiated = false;
  }

  GetContentClient()->browser()->OverrideOpenURLParams(current_site_instance,
                                                       &params);

  if (delegate_)
    delegate_->RequestOpenURL(render_frame_host, params);
}
