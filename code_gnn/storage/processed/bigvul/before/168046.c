std::vector<Suggestion> AutofillManager::GetCreditCardSuggestions(
    const FormFieldData& field,
    const AutofillType& type,
    bool* is_all_server_suggestions) const {
  credit_card_form_event_logger_->OnDidPollSuggestions(field);

  std::vector<Suggestion> suggestions =
      personal_data_->GetCreditCardSuggestions(
          type, SanitizeCreditCardFieldValue(field.value),
          client_->AreServerCardsSupported());
  const std::vector<CreditCard*> cards_to_suggest =
      personal_data_->GetCreditCardsToSuggest(
          client_->AreServerCardsSupported());
  for (const CreditCard* credit_card : cards_to_suggest) {
    if (!credit_card->bank_name().empty()) {
      credit_card_form_event_logger_->SetBankNameAvailable();
      break;
    }
  }

  *is_all_server_suggestions = true;
  for (const CreditCard* credit_card : cards_to_suggest) {
    if (credit_card->record_type() == CreditCard::LOCAL_CARD)
      *is_all_server_suggestions = false;
  }

  for (size_t i = 0; i < suggestions.size(); i++) {
    suggestions[i].frontend_id =
        MakeFrontendID(suggestions[i].backend_id, std::string());
  }
  return suggestions;
}
