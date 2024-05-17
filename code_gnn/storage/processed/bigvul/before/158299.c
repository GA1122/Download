void RenderWidgetHostImpl::InitForFrame() {
  DCHECK(process_->IsInitializedAndNotDead());
  renderer_initialized_ = true;

  if (view_)
    view_->OnRenderWidgetInit();
}
