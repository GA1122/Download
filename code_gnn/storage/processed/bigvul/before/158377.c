void RenderWidgetHostImpl::StopFling() {
  if (input_router_)
    input_router_->StopFling();
}
