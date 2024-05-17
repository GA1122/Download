void AuthenticatorSheetModelBase::OnBack() {
  if (dialog_model())
    dialog_model()->Back();
}
