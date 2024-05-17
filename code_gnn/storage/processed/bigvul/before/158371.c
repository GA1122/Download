void RenderWidgetHostImpl::SetWantsAnimateOnlyBeginFrames() {
  if (view_)
    view_->SetWantsAnimateOnlyBeginFrames();
}
