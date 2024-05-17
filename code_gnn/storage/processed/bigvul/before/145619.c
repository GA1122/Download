void NavigationRequest::CreateNavigationHandle() {
  DCHECK_EQ(frame_tree_node_->navigation_request(), this);
  FrameTreeNode* frame_tree_node = frame_tree_node_;

  std::vector<GURL> redirect_chain;
  if (!begin_params_->client_side_redirect_url.is_empty())
    redirect_chain.push_back(begin_params_->client_side_redirect_url);
  redirect_chain.push_back(common_params_.url);

  bool is_external_protocol =
      !GetContentClient()->browser()->IsHandledURL(common_params_.url);

  net::HttpRequestHeaders headers;
  headers.AddHeadersFromString(begin_params_->headers);

  std::unique_ptr<NavigationHandleImpl> navigation_handle =
      NavigationHandleImpl::Create(
          common_params_.url, redirect_chain, frame_tree_node_,
          !browser_initiated_,
          FrameMsg_Navigate_Type::IsSameDocument(
              common_params_.navigation_type),
          common_params_.navigation_start, nav_entry_id_,
          common_params_.started_from_context_menu,
          common_params_.should_check_main_world_csp,
          begin_params_->is_form_submission, std::move(navigation_ui_data_),
          common_params_.method, std::move(headers), common_params_.post_data,
          Referrer::SanitizeForRequest(common_params_.url,
                                       common_params_.referrer),
          common_params_.has_user_gesture, common_params_.transition,
          is_external_protocol, begin_params_->request_context_type,
          begin_params_->mixed_content_context_type,
          common_params_.input_start);

  if (!frame_tree_node->navigation_request()) {
    return;
  }

  navigation_handle_ = std::move(navigation_handle);

  if (!begin_params_->searchable_form_url.is_empty()) {
    navigation_handle_->set_searchable_form_url(
        begin_params_->searchable_form_url);
    navigation_handle_->set_searchable_form_encoding(
        begin_params_->searchable_form_encoding);
  }

  if (common_params_.source_location) {
    navigation_handle_->set_source_location(
        common_params_.source_location.value());
  }
}
