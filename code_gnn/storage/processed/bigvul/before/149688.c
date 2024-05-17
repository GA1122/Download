void SaveCardBubbleControllerImpl::OnSaveButton(const base::string16& cvc) {
  if (!cvc.empty()) {
    DCHECK(ShouldRequestCvcFromUser());
    DCHECK(InputCvcIsValid(cvc));
    base::TrimWhitespace(cvc, base::TRIM_ALL, &cvc_entered_by_user_);
  }
  save_card_callback_.Run();
  save_card_callback_.Reset();
  AutofillMetrics::LogSaveCardPromptMetric(
      AutofillMetrics::SAVE_CARD_PROMPT_END_ACCEPTED, is_uploading_, is_reshow_,
      pref_service_->GetInteger(
          prefs::kAutofillAcceptSaveCreditCardPromptState));
  pref_service_->SetInteger(
      prefs::kAutofillAcceptSaveCreditCardPromptState,
      prefs::PREVIOUS_SAVE_CREDIT_CARD_PROMPT_USER_DECISION_ACCEPTED);
}
