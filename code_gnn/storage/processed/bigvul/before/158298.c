void RenderWidgetHostImpl::Init() {
  DCHECK(process_->IsInitializedAndNotDead());

  renderer_initialized_ = true;

  SendScreenRects();
  SynchronizeVisualProperties();

  if (owner_delegate_)
    owner_delegate_->RenderWidgetDidInit();

  if (view_)
    view_->OnRenderWidgetInit();
}
