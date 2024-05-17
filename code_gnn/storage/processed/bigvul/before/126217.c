WebContents* Browser::OpenURLFromTab(WebContents* source,
                                     const OpenURLParams& params) {
  chrome::NavigateParams nav_params(this, params.url, params.transition);
  nav_params.source_contents = chrome::GetTabContentsAt(this,
      tab_strip_model_->GetIndexOfWebContents(source));
  nav_params.referrer = params.referrer;
  nav_params.extra_headers = params.extra_headers;
  nav_params.disposition = params.disposition;
  nav_params.tabstrip_add_types = TabStripModel::ADD_NONE;
  nav_params.window_action = chrome::NavigateParams::SHOW_WINDOW;
  nav_params.user_gesture = true;
  nav_params.override_encoding = params.override_encoding;
  nav_params.is_renderer_initiated = params.is_renderer_initiated;
  nav_params.transferred_global_request_id =
      params.transferred_global_request_id;
  chrome::Navigate(&nav_params);

  return nav_params.target_contents ?
      nav_params.target_contents->web_contents() : NULL;
}
