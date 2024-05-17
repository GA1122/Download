void RenderWidgetHostImpl::StartNewContentRenderingTimeout() {
  if (received_paint_after_load_) {
    received_paint_after_load_ = false;
    return;
  }

  new_content_rendering_timeout_->Start(new_content_rendering_delay_);
}
