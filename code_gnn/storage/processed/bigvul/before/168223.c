void LoadURLInContents(WebContents* target_contents,
                       const GURL& url,
                       NavigateParams* params) {
  NavigationController::LoadURLParams load_url_params(url);
  load_url_params.source_site_instance = params->source_site_instance;
  load_url_params.referrer = params->referrer;
  load_url_params.frame_name = params->frame_name;
  load_url_params.frame_tree_node_id = params->frame_tree_node_id;
  load_url_params.redirect_chain = params->redirect_chain;
  load_url_params.transition_type = params->transition;
  load_url_params.extra_headers = params->extra_headers;
  load_url_params.should_replace_current_entry =
      params->should_replace_current_entry;
  load_url_params.is_renderer_initiated = params->is_renderer_initiated;
  load_url_params.started_from_context_menu = params->started_from_context_menu;
  load_url_params.has_user_gesture = params->user_gesture;
  load_url_params.blob_url_loader_factory = params->blob_url_loader_factory;

  if (params->frame_tree_node_id ==
      content::RenderFrameHost::kNoFrameTreeNodeId) {
    load_url_params.navigation_ui_data =
        ChromeNavigationUIData::CreateForMainFrameNavigation(
            target_contents, params->disposition);
  }

  if (params->uses_post) {
    load_url_params.load_type = NavigationController::LOAD_TYPE_HTTP_POST;
    load_url_params.post_data = params->post_data;
  }

  target_contents->GetController().LoadURLWithParams(load_url_params);
}
