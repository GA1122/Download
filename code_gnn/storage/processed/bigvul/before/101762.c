TabContents* Browser::OpenURLFromTab(TabContents* source,
                                     const OpenURLParams& params) {
  browser::NavigateParams nav_params(this, params.url, params.transition);
  nav_params.source_contents =
      tabstrip_model()->GetTabContentsAt(
          tabstrip_model()->GetWrapperIndex(source));
  nav_params.referrer = params.referrer;
  nav_params.disposition = params.disposition;
  nav_params.tabstrip_add_types = TabStripModel::ADD_NONE;
  nav_params.window_action = browser::NavigateParams::SHOW_WINDOW;
  nav_params.user_gesture = true;
  nav_params.override_encoding = params.override_encoding;
  browser::Navigate(&nav_params);

  return nav_params.target_contents ?
      nav_params.target_contents->tab_contents() : NULL;
}
