void RenderFrameImpl::UpdateNavigationState(DocumentState* document_state,
                                            bool was_within_same_document,
                                            bool content_initiated) {
  if (!pending_navigation_params_ || content_initiated) {
    document_state->set_navigation_state(
        NavigationStateImpl::CreateContentInitiated());
    return;
  }

  DCHECK(!pending_navigation_params_->common_params.navigation_start.is_null());
  document_state->set_navigation_state(CreateNavigationStateFromPending());

  if (!was_within_same_document) {
    const CommonNavigationParams& common_params =
        pending_navigation_params_->common_params;
    bool load_data = !common_params.base_url_for_data_url.is_empty() &&
                     !common_params.history_url_for_data_url.is_empty() &&
                     common_params.url.SchemeIs(url::kDataScheme);
    document_state->set_was_load_data_with_base_url_request(load_data);
    if (load_data)
      document_state->set_data_url(common_params.url);
  }

  pending_navigation_params_.reset();
}
