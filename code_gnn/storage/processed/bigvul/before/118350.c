void CardUnmaskPromptViews::ContentsChanged(
    views::Textfield* sender,
    const base::string16& new_contents) {
  if (controller_->InputCvcIsValid(new_contents))
    cvc_input_->SetInvalid(false);

  GetDialogClientView()->UpdateDialogButtons();
}
