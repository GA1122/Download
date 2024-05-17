void RenderWidgetHostImpl::Init() {
  DCHECK(process_->HasConnection());

  renderer_initialized_ = true;

  SendScreenRects();
  WasResized();

  if (owner_delegate_)
    owner_delegate_->RenderWidgetDidInit();

  if (view_)
    view_->OnRenderWidgetInit();
}
