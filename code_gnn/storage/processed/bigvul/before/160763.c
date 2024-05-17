void RenderViewImpl::CloseForFrame() {
  DCHECK(frame_widget_);
  frame_widget_->Close();
  frame_widget_ = nullptr;
}
