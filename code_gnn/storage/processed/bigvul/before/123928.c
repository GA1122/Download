void RenderViewImpl::PopulateDocumentStateFromPending(
    DocumentState* document_state) {
  const ViewMsg_Navigate_Params& params = *pending_navigation_params_.get();
  document_state->set_request_time(params.request_time);

  if (!params.url.SchemeIs(chrome::kJavaScriptScheme) &&
      params.navigation_type == ViewMsg_Navigate_Type::RESTORE) {
    document_state->set_cache_policy_override(
        WebURLRequest::UseProtocolCachePolicy);
  }

  if (IsReload(params))
    document_state->set_load_type(DocumentState::RELOAD);
  else if (!params.state.empty())
    document_state->set_load_type(DocumentState::HISTORY_LOAD);
  else
    document_state->set_load_type(DocumentState::NORMAL_LOAD);

  document_state->set_referrer_policy(params.referrer.policy);
  document_state->set_is_overriding_user_agent(params.is_overriding_user_agent);
  document_state->set_must_reset_scroll_and_scale_state(
      params.navigation_type ==
          ViewMsg_Navigate_Type::RELOAD_ORIGINAL_REQUEST_URL);
  document_state->set_can_load_local_resources(params.can_load_local_resources);
}
