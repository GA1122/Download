void CardUnmaskPromptViews::SetInputsEnabled(bool enabled) {
  cvc_input_->SetEnabled(enabled);
  if (storage_checkbox_)
    storage_checkbox_->SetEnabled(enabled);
  if (month_input_)
    month_input_->SetEnabled(enabled);
  if (year_input_)
    year_input_->SetEnabled(enabled);
}
