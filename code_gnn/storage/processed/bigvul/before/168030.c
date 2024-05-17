void AutofillManager::DidShowSuggestions(bool has_autofill_suggestions,
                                         const FormData& form,
                                         const FormFieldData& field) {
  if (test_delegate_)
    test_delegate_->DidShowSuggestions();
  FormStructure* form_structure = nullptr;
  AutofillField* autofill_field = nullptr;
  if (!GetCachedFormAndField(form, field, &form_structure, &autofill_field))
    return;

  if (has_autofill_suggestions) {
    AutofillMetrics::LogUserHappinessMetric(
        AutofillMetrics::SUGGESTIONS_SHOWN, autofill_field->Type().group(),
        client_->GetSecurityLevelForUmaHistograms());

    if (!did_show_suggestions_) {
      did_show_suggestions_ = true;
      AutofillMetrics::LogUserHappinessMetric(
          AutofillMetrics::SUGGESTIONS_SHOWN_ONCE,
          autofill_field->Type().group(),
          client_->GetSecurityLevelForUmaHistograms());
    }

    if (autofill_field->Type().group() == CREDIT_CARD) {
      credit_card_form_event_logger_->OnDidShowSuggestions(
          *form_structure, *autofill_field,
          form_structure->form_parsed_timestamp());
    } else {
      address_form_event_logger_->OnDidShowSuggestions(
          *form_structure, *autofill_field,
          form_structure->form_parsed_timestamp());
    }
  }
}
