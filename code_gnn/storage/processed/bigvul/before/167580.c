bool RenderFrameHostManager::ShouldSwapBrowsingInstancesForNavigation(
    const GURL& current_effective_url,
    bool current_is_view_source_mode,
    SiteInstance* new_site_instance,
    const GURL& new_effective_url,
    bool new_is_view_source_mode,
    bool is_failure) const {
  if (!frame_tree_node_->IsMainFrame())
    return false;

  if (is_failure && SiteIsolationPolicy::IsErrorPageIsolationEnabled(
                        frame_tree_node_->IsMainFrame())) {
    return false;
  }

  if (new_site_instance) {
    return !new_site_instance->IsRelatedSiteInstance(
        render_frame_host_->GetSiteInstance());
  }

  BrowserContext* browser_context =
      delegate_->GetControllerForRenderManager().GetBrowserContext();

  if (IsRendererDebugURL(new_effective_url))
    return false;

  DCHECK_EQ(browser_context,
            render_frame_host_->GetSiteInstance()->GetBrowserContext());
  if (browser_context !=
      render_frame_host_->GetSiteInstance()->GetBrowserContext()) {
    return true;
  }

  if (ChildProcessSecurityPolicyImpl::GetInstance()->HasWebUIBindings(
          render_frame_host_->GetProcess()->GetID()) ||
      WebUIControllerFactoryRegistry::GetInstance()->UseWebUIBindingsForURL(
          browser_context, current_effective_url)) {
    if (!WebUIControllerFactoryRegistry::GetInstance()->IsURLAcceptableForWebUI(
            browser_context, new_effective_url)) {
      return true;
    }

    if (WebUIControllerFactoryRegistry::GetInstance()->GetWebUIType(
            browser_context, current_effective_url) !=
        WebUIControllerFactoryRegistry::GetInstance()->GetWebUIType(
            browser_context, new_effective_url)) {
      return true;
    }
  } else {
    if (WebUIControllerFactoryRegistry::GetInstance()->UseWebUIBindingsForURL(
            browser_context, new_effective_url)) {
      return true;
    }
  }

  if (GetContentClient()->browser()->ShouldSwapBrowsingInstancesForNavigation(
          render_frame_host_->GetSiteInstance(),
          current_effective_url, new_effective_url)) {
    return true;
  }

  if (current_is_view_source_mode != new_is_view_source_mode)
    return true;

  return false;
}
