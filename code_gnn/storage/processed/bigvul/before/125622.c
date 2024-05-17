void RenderViewHostImpl::GotFocus() {
  RenderWidgetHostImpl::GotFocus();   

  RenderViewHostDelegateView* view = delegate_->GetDelegateView();
  if (view)
    view->GotFocus();
}
