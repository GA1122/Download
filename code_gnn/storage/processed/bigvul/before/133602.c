void WebContentsImpl::OnHideValidationMessage() {
  if (delegate_)
    delegate_->HideValidationMessage(this);
}
