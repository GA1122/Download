bool PermissionsBubbleDialogDelegateView::Cancel() {
  if (owner_) {
    owner_->TogglePersist(!persist_checkbox_ || persist_checkbox_->checked());
    owner_->Deny();
  }
  return true;
}
