bool AutofillManager::RefreshDataModels() {
  if (!IsAutofillEnabled())
    return false;

  const std::vector<AutofillProfile*>& profiles = personal_data_->GetProfiles();
  const std::vector<CreditCard*>& credit_cards =
      personal_data_->GetCreditCards();

  {
    size_t server_record_type_count = 0;
    size_t local_record_type_count = 0;
    for (CreditCard* credit_card : credit_cards) {
      if (credit_card->record_type() == CreditCard::LOCAL_CARD)
        local_record_type_count++;
      else
        server_record_type_count++;
    }
    credit_card_form_event_logger_->set_server_record_type_count(
        server_record_type_count);
    credit_card_form_event_logger_->set_local_record_type_count(
        local_record_type_count);
    credit_card_form_event_logger_->set_is_context_secure(
        client_->IsContextSecure());
  }
  {
    size_t server_record_type_count = 0;
    size_t local_record_type_count = 0;
    for (AutofillProfile* profile : profiles) {
      if (profile->record_type() == AutofillProfile::LOCAL_PROFILE)
        local_record_type_count++;
      else if (profile->record_type() == AutofillProfile::SERVER_PROFILE)
        server_record_type_count++;
    }
    address_form_event_logger_->set_server_record_type_count(
        server_record_type_count);
    address_form_event_logger_->set_local_record_type_count(
        local_record_type_count);
  }

  if (profiles.empty() && credit_cards.empty())
    return false;

  return true;
}
