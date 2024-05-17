void RenderFrameImpl::PopulateDocumentStateFromPending(
    DocumentState* document_state) {
  InternalDocumentStateData* internal_data =
      InternalDocumentStateData::FromDocumentState(document_state);

  if (!pending_navigation_params_->common_params.url.SchemeIs(
          url::kJavaScriptScheme) &&
      pending_navigation_params_->common_params.navigation_type ==
          FrameMsg_Navigate_Type::RESTORE) {
    internal_data->set_cache_policy_override(
        blink::mojom::FetchCacheMode::kDefault);
  }

  internal_data->set_is_overriding_user_agent(
      pending_navigation_params_->request_params.is_overriding_user_agent);
  internal_data->set_must_reset_scroll_and_scale_state(
      pending_navigation_params_->common_params.navigation_type ==
      FrameMsg_Navigate_Type::RELOAD_ORIGINAL_REQUEST_URL);
  document_state->set_can_load_local_resources(
      pending_navigation_params_->request_params.can_load_local_resources);
}
