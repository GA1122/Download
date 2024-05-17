void RenderWidgetHostImpl::LostFocus() {
  Blur();
  if (owner_delegate_)
    owner_delegate_->RenderWidgetLostFocus();
  if (delegate_)
    delegate_->RenderWidgetLostFocus(this);
}
