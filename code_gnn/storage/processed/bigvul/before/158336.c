void RenderWidgetHostImpl::OnUpdateDragCursor(WebDragOperation current_op) {
  if (delegate_->OnUpdateDragCursor())
    return;

  RenderViewHostDelegateView* view = delegate_->GetDelegateView();
  if (view)
    view->UpdateDragCursor(current_op);
}
