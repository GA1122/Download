void RenderWidgetHostImpl::SetNeedsFlush() {
  if (view_)
    view_->OnSetNeedsFlushInput();
}
