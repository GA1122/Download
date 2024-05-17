bool CardUnmaskPromptViews::IsDialogButtonEnabled(
    ui::DialogButton button) const {
  if (button == ui::DIALOG_BUTTON_CANCEL)
    return true;

  DCHECK_EQ(ui::DIALOG_BUTTON_OK, button);

  return cvc_input_->enabled() &&
         controller_->InputCvcIsValid(cvc_input_->text()) &&
         ExpirationDateIsValid();
}
