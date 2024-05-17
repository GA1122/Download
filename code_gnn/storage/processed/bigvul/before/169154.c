void RenderFrameHostImpl::Init() {
  ResumeBlockedRequestsForFrame();
  if (!waiting_for_init_)
    return;

  waiting_for_init_ = false;
  if (pending_navigate_) {
    frame_tree_node()->navigator()->OnBeginNavigation(
        frame_tree_node(), pending_navigate_->common_params,
        std::move(pending_navigate_->begin_params),
        std::move(pending_navigate_->blob_url_loader_factory));
    pending_navigate_.reset();
  }
}
