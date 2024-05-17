void RenderViewHostImpl::OnRenderViewReady() {
  render_view_termination_status_ = base::TERMINATION_STATUS_STILL_RUNNING;
  SendScreenRects();
  WasResized();
  delegate_->RenderViewReady(this);
}
