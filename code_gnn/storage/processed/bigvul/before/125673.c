void RenderViewHostImpl::OnRouteCloseEvent() {
  delegate_->RouteCloseEvent(this);
}
