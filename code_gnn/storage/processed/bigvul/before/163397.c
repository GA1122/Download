bool RenderThreadImpl::RendererIsHidden() const {
  return widget_count_ > 0 && hidden_widget_count_ == widget_count_;
}
