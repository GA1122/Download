void AutofillManager::OnFormSubmittedImpl(const FormData& form,
                                          bool known_success,
                                          SubmissionSource source,
                                          base::TimeTicks timestamp) {
  if (source == SubmissionSource::PROBABLY_FORM_SUBMITTED)
    return;

  std::unique_ptr<FormStructure> submitted_form = ValidateSubmittedForm(form);
  if (!submitted_form) {
    autocomplete_history_manager_->OnWillSubmitForm(form);
    return;
  }

  FormData form_for_autocomplete = submitted_form->ToFormData();
  for (size_t i = 0; i < submitted_form->field_count(); ++i) {
    if (submitted_form->field(i)->Type().GetStorableType() ==
        CREDIT_CARD_VERIFICATION_CODE) {
      form_for_autocomplete.fields[i].should_autocomplete = false;
    }
  }
  autocomplete_history_manager_->OnWillSubmitForm(form_for_autocomplete);

  address_form_event_logger_->OnWillSubmitForm();
  if (IsCreditCardAutofillEnabled())
    credit_card_form_event_logger_->OnWillSubmitForm();

  MaybeStartVoteUploadProcess(std::move(submitted_form), timestamp,
                               true);

  submitted_form = ValidateSubmittedForm(form);
  DCHECK(submitted_form);
  if (!submitted_form)
    return;

  CreditCard credit_card =
      form_data_importer_->ExtractCreditCardFromForm(*submitted_form);
  AutofillMetrics::CardNumberStatus card_number_status =
      GetCardNumberStatus(credit_card);

  address_form_event_logger_->OnFormSubmitted( false,
                                              card_number_status);
  if (IsCreditCardAutofillEnabled())
    credit_card_form_event_logger_->OnFormSubmitted(enable_ablation_logging_,
                                                    card_number_status);

  if (!submitted_form->IsAutofillable())
    return;

  form_data_importer_->ImportFormData(*submitted_form,
                                      IsCreditCardAutofillEnabled());
}
