base::string16 ExtensionInstallDialogView::GetDialogButtonLabel(
    ui::DialogButton button) const {
  switch (button) {
    case ui::DIALOG_BUTTON_OK:
      return prompt_->GetAcceptButtonLabel();
    case ui::DIALOG_BUTTON_CANCEL:
      return prompt_->GetAbortButtonLabel();
    default:
      NOTREACHED();
      return base::string16();
  }
}
