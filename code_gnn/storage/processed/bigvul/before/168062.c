void AutofillManager::OnDidFillAutofillFormData(const FormData& form,
                                                const TimeTicks timestamp) {
  if (test_delegate_)
    test_delegate_->DidFillFormData();

  UpdatePendingForm(form);

  FormStructure* form_structure = nullptr;
  std::set<FormType> form_types;
  if (FindCachedForm(form, &form_structure)) {
    form_types = form_structure->GetFormTypes();
  }
  AutofillMetrics::LogUserHappinessMetric(
      AutofillMetrics::USER_DID_AUTOFILL, form_types,
      client_->GetSecurityLevelForUmaHistograms());
  if (!user_did_autofill_) {
    user_did_autofill_ = true;
    AutofillMetrics::LogUserHappinessMetric(
        AutofillMetrics::USER_DID_AUTOFILL_ONCE, form_types,
        client_->GetSecurityLevelForUmaHistograms());
  }

  UpdateInitialInteractionTimestamp(timestamp);
}
