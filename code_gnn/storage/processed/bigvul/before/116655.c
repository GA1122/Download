void RenderViewImpl::PopulateStateFromPendingNavigationParams(
    DocumentState* document_state) {
  const ViewMsg_Navigate_Params& params = *pending_navigation_params_.get();

  if (document_state->request_time().is_null())
    document_state->set_request_time(params.request_time);

  if (!params.url.SchemeIs(chrome::kJavaScriptScheme)) {
    NavigationState* navigation_state = NavigationState::CreateBrowserInitiated(
        params.page_id,
        params.pending_history_list_offset,
        params.transition);
    navigation_state->set_transferred_request_child_id(
        params.transferred_request_child_id);
    navigation_state->set_transferred_request_request_id(
        params.transferred_request_request_id);
    if (params.navigation_type == ViewMsg_Navigate_Type::RESTORE) {
      document_state->set_cache_policy_override(
          WebURLRequest::UseProtocolCachePolicy);
    }
    document_state->set_navigation_state(navigation_state);
  } else {
    document_state->set_navigation_state(
        NavigationState::CreateContentInitiated());
  }

  if (IsReload(params))
    document_state->set_load_type(DocumentState::RELOAD);
  else if (!params.state.empty())
    document_state->set_load_type(DocumentState::HISTORY_LOAD);
  else
    document_state->set_load_type(DocumentState::NORMAL_LOAD);

  pending_navigation_params_.reset();
}
