bool RenderWidgetHostImpl::HasGestureStopped() {
  return !input_router_->HasPendingEvents();
}
