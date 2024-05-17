NavigationState* RenderViewImpl::CreateNavigationStateFromPending() {
  const ViewMsg_Navigate_Params& params = *pending_navigation_params_.get();
  NavigationState* navigation_state = NULL;

  if (!params.url.SchemeIs(chrome::kJavaScriptScheme)) {
    navigation_state = NavigationState::CreateBrowserInitiated(
        params.page_id,
        params.pending_history_list_offset,
        params.transition);
    navigation_state->set_transferred_request_child_id(
        params.transferred_request_child_id);
    navigation_state->set_transferred_request_request_id(
        params.transferred_request_request_id);
    navigation_state->set_allow_download(params.allow_download);
  } else {
    navigation_state = NavigationState::CreateContentInitiated();
  }
  return navigation_state;
}
