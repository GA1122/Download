void NavigationRequest::CreateNavigationHandle(bool is_for_commit) {
  DCHECK(frame_tree_node_->navigation_request() == this || is_for_commit);
  FrameTreeNode* frame_tree_node = frame_tree_node_;

  std::vector<GURL> redirect_chain;
  if (!begin_params_->client_side_redirect_url.is_empty()) {
    redirect_chain.push_back(begin_params_->client_side_redirect_url);
  } else if (!commit_params_.redirects.empty()) {
    for (const auto& url : commit_params_.redirects)
      redirect_chain.push_back(url);
  }

  if (!is_for_commit)
    redirect_chain.push_back(common_params_.url);

  net::HttpRequestHeaders headers;
  headers.AddHeadersFromString(begin_params_->headers);

  std::unique_ptr<NavigationHandleImpl> navigation_handle =
      base::WrapUnique(new NavigationHandleImpl(
          this, redirect_chain,
          FrameMsg_Navigate_Type::IsSameDocument(
              common_params_.navigation_type),
          nav_entry_id_, std::move(navigation_ui_data_), std::move(headers),
          Referrer::SanitizeForRequest(common_params_.url,
                                       common_params_.referrer)));

  if (!frame_tree_node->navigation_request() && !is_for_commit) {
    return;
  }

  navigation_handle_ = std::move(navigation_handle);
}
