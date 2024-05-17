void NavigatorImpl::CheckWebUIRendererDoesNotDisplayNormalURL(
    RenderFrameHostImpl* render_frame_host,
    const GURL& url) {
  int enabled_bindings = render_frame_host->GetEnabledBindings();
  bool is_allowed_in_web_ui_renderer =
      WebUIControllerFactoryRegistry::GetInstance()->IsURLAcceptableForWebUI(
          render_frame_host->frame_tree_node()
              ->navigator()
              ->GetController()
              ->GetBrowserContext(),
          url);
  if ((enabled_bindings & kWebUIBindingsPolicyMask) &&
      !is_allowed_in_web_ui_renderer) {
    FrameTreeNode* root_node =
        render_frame_host->frame_tree_node()->frame_tree()->root();
    GetContentClient()->SetActiveURL(
        url, root_node->current_url().possibly_invalid_spec());
    CHECK(0);
  }
}
