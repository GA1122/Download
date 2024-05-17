bool AutofillDialogViews::Accept() {
  if (ValidateForm())
    return delegate_->OnAccept();

  DCHECK(!validity_map_.empty());
  FocusInitialView();

  return false;
}
