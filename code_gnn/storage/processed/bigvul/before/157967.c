void RenderViewImpl::OverrideCloseForWidget() {
  DCHECK(frame_widget_);
  frame_widget_->Close();
  frame_widget_ = nullptr;
}
