void NavigatorImpl::DidStartProvisionalLoad(
    RenderFrameHostImpl* render_frame_host,
    const GURL& url,
    const base::TimeTicks& navigation_start) {
  bool is_main_frame = render_frame_host->frame_tree_node()->IsMainFrame();
  bool is_error_page = (url.spec() == kUnreachableWebDataURL);
  bool is_iframe_srcdoc = (url.spec() == kAboutSrcDocURL);
  GURL validated_url(url);
  RenderProcessHost* render_process_host = render_frame_host->GetProcess();
  render_process_host->FilterURL(false, &validated_url);

  ChildProcessSecurityPolicyImpl* policy =
      ChildProcessSecurityPolicyImpl::GetInstance();
  if (render_process_host->IsForGuestsOnly() &&
      !policy->IsWebSafeScheme(validated_url.scheme())) {
    validated_url = GURL(url::kAboutBlankURL);
  }

  if (is_main_frame && !is_error_page) {
    DidStartMainFrameNavigation(validated_url,
                                render_frame_host->GetSiteInstance(),
                                render_frame_host->navigation_handle());
  }

  if (delegate_) {
    delegate_->DidStartProvisionalLoad(render_frame_host, validated_url,
                                       is_error_page, is_iframe_srcdoc);
  }

  if (is_error_page || IsBrowserSideNavigationEnabled())
    return;

  if (render_frame_host->navigation_handle()) {
    if (render_frame_host->navigation_handle()->is_transferring()) {
      DCHECK_EQ(url, render_frame_host->navigation_handle()->GetURL());
      render_frame_host->navigation_handle()->set_is_transferring(false);
      return;
    }

    render_frame_host->SetNavigationHandle(
        std::unique_ptr<NavigationHandleImpl>());
  }

  bool is_renderer_initiated = true;
  int pending_nav_entry_id = 0;
  bool started_from_context_menu = false;
  NavigationEntryImpl* pending_entry = controller_->GetPendingEntry();
  if (pending_entry) {
    is_renderer_initiated = pending_entry->is_renderer_initiated();
    pending_nav_entry_id = pending_entry->GetUniqueID();
    started_from_context_menu = pending_entry->has_started_from_context_menu();
  }

  render_frame_host->SetNavigationHandle(NavigationHandleImpl::Create(
      validated_url, render_frame_host->frame_tree_node(),
      is_renderer_initiated,
      false,              
      is_iframe_srcdoc,   
      navigation_start, pending_nav_entry_id, started_from_context_menu));
}
