void PermissionPromptImpl::UpdateAnchorPosition() {
  DCHECK(browser_);
  DCHECK(browser_->window());

  if (bubble_delegate_) {
    bubble_delegate_->set_parent_window(
        platform_util::GetViewForWindow(browser_->window()->GetNativeWindow()));
    bubble_delegate_->UpdateAnchor();
  }
}
