void RenderWidgetHostImpl::OnFirstPaintAfterLoad() {
  if (new_content_rendering_timeout_->IsRunning()) {
    new_content_rendering_timeout_->Stop();
  } else {
    received_paint_after_load_ = true;
  }
}
