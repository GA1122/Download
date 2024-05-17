bool CardUnmaskPromptViews::Accept() {
  if (!controller_)
    return true;

  controller_->OnUnmaskResponse(
      cvc_input_->text(),
      month_input_ ? month_input_->GetTextForRow(month_input_->selected_index())
                   : base::string16(),
      year_input_ ? year_input_->GetTextForRow(year_input_->selected_index())
                  : base::string16(),
      storage_checkbox_ ? storage_checkbox_->checked() : false);
  return false;
}
