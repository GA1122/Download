void PasswordAutofillManager::OnShowNotSecureWarning(
    base::i18n::TextDirection text_direction,
    const gfx::RectF& bounds) {
  DCHECK(security_state::IsHttpWarningInFormEnabled());
  if (!autofill_client_)
    return;

  std::vector<autofill::Suggestion> suggestions;
  autofill::Suggestion http_warning_suggestion = CreateFormNotSecureWarning();
  suggestions.insert(suggestions.begin(), http_warning_suggestion);

  autofill_client_->ShowAutofillPopup(bounds, text_direction, suggestions,
                                      weak_ptr_factory_.GetWeakPtr());
}
