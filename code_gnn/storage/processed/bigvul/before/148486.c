void WebContentsImpl::OnHideValidationMessage(RenderViewHostImpl* source) {
  if (delegate_)
    delegate_->HideValidationMessage(this);
}
