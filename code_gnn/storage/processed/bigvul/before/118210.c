void AutofillDialogViews::DeleteDelegate() {
  window_ = NULL;
  delegate_->ViewClosed();
}
