void RenderFrameHostImpl::Init() {
  ResumeBlockedRequestsForFrame();
  if (!waiting_for_init_)
    return;

  waiting_for_init_ = false;
  if (pendinging_navigate_) {
    frame_tree_node()->navigator()->OnBeginNavigation(
        frame_tree_node(), pendinging_navigate_->first,
        pendinging_navigate_->second);
    pendinging_navigate_.reset();
  }
}
