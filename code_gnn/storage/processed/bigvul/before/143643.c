void RenderWidgetHostImpl::GotFocus() {
  Focus();
  if (owner_delegate_)
    owner_delegate_->RenderWidgetGotFocus();
  if (delegate_)
    delegate_->RenderWidgetGotFocus(this);
}
