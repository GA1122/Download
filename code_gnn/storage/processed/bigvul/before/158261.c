bool RenderWidgetHostImpl::FlingCancellationIsDeferred() const {
  if (input_router_)
    return input_router_->FlingCancellationIsDeferred();

  return false;
}
