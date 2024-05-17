base::string16 AutofillDialogViews::GetDialogButtonLabel(
    ui::DialogButton button) const {
  return button == ui::DIALOG_BUTTON_OK ?
      delegate_->ConfirmButtonText() : delegate_->CancelButtonText();
}
