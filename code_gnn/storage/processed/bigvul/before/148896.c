void RenderFrameHostManager::OnCrossSiteResponse(
    RenderFrameHostImpl* transferring_render_frame_host,
    const GlobalRequestID& global_request_id,
    const std::vector<GURL>& transfer_url_chain,
    const Referrer& referrer,
    ui::PageTransition page_transition,
    bool should_replace_current_entry) {
  DCHECK(transferring_render_frame_host == pending_render_frame_host_.get() ||
         transferring_render_frame_host == render_frame_host_.get());

  bool frame_tree_node_was_loading = frame_tree_node_->IsLoading();

  transfer_navigation_handle_ =
      transferring_render_frame_host->PassNavigationHandleOwnership();
  CHECK(transfer_navigation_handle_);

  transferring_render_frame_host->set_is_loading(false);

  CHECK(transfer_url_chain.size());
  GURL transfer_url = transfer_url_chain.back();
  std::vector<GURL> rest_of_chain = transfer_url_chain;
  rest_of_chain.pop_back();

  std::string extra_headers;

  transferring_render_frame_host->frame_tree_node()
      ->navigator()
      ->RequestTransferURL(
          transferring_render_frame_host, transfer_url, nullptr, rest_of_chain,
          referrer, page_transition, global_request_id,
          should_replace_current_entry,
          transfer_navigation_handle_->IsPost() ? "POST" : "GET",
          transfer_navigation_handle_->resource_request_body(), extra_headers);

  if (transfer_navigation_handle_) {
    transfer_navigation_handle_->set_net_error_code(net::ERR_ABORTED);
    transfer_navigation_handle_->set_is_transferring(false);
    transfer_navigation_handle_.reset();
  }

  if (!frame_tree_node_->IsLoading() && frame_tree_node_was_loading)
    frame_tree_node_->DidStopLoading();
}
