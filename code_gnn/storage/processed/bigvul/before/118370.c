void CardUnmaskPromptViews::OnPerformAction(views::Combobox* combobox) {
  if (ExpirationDateIsValid()) {
    if (month_input_->invalid()) {
      month_input_->SetInvalid(false);
      year_input_->SetInvalid(false);
      error_label_->SetMultiLine(false);
      SetRetriableErrorMessage(base::string16());
    }
  } else if (month_input_->selected_index() !=
                 month_combobox_model_.GetDefaultIndex() &&
             year_input_->selected_index() !=
                 year_combobox_model_.GetDefaultIndex()) {
    month_input_->SetInvalid(true);
    year_input_->SetInvalid(true);
    error_label_->SetMultiLine(true);
    SetRetriableErrorMessage(l10n_util::GetStringUTF16(
        IDS_AUTOFILL_CARD_UNMASK_INVALID_EXPIRATION_DATE));
  }

  GetDialogClientView()->UpdateDialogButtons();
}
