void RenderWidgetHostImpl::ResetSizeAndRepaintPendingFlags() {
  resize_ack_pending_ = false;
  if (repaint_ack_pending_) {
    TRACE_EVENT_ASYNC_END0(
        "renderer_host", "RenderWidgetHostImpl::repaint_ack_pending_", this);
  }
  repaint_ack_pending_ = false;
  if (old_resize_params_)
    old_resize_params_->new_size = gfx::Size();
}
