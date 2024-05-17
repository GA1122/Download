void RenderWidgetHostImpl::DidStopFlinging() {
  if (view_)
    view_->DidStopFlinging();
}
