bool PermissionsBubbleDialogDelegateView::Accept() {
  if (owner_) {
    owner_->TogglePersist(!persist_checkbox_ || persist_checkbox_->checked());
    owner_->Accept();
  }
  return true;
}
