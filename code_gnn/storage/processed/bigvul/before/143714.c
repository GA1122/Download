bool RenderWidgetHostImpl::ShouldDropInputEvents() const {
  return ignore_input_events_ || process_->IgnoreInputEvents() || !delegate_;
}
