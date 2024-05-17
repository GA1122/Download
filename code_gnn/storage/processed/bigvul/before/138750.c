void RenderFrameHostImpl::OnDidStopLoading() {
  if (!is_loading_) {
    LOG(WARNING) << "OnDidStopLoading was called twice.";
    return;
  }

  is_loading_ = false;
  navigation_handle_.reset();

  if (is_active())
    frame_tree_node_->DidStopLoading();
}
