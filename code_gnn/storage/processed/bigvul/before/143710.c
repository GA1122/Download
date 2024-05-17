void RenderWidgetHostImpl::SetIsLoading(bool is_loading) {
  if (owner_delegate_)
    owner_delegate_->RenderWidgetWillSetIsLoading(is_loading);

  is_loading_ = is_loading;
  if (view_)
    view_->SetIsLoading(is_loading);
}
