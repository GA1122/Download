autofill::Suggestion PasswordAutofillManager::CreateFormNotSecureWarning() {
  autofill::Suggestion http_warning_suggestion(
      l10n_util::GetStringUTF8(IDS_AUTOFILL_LOGIN_HTTP_WARNING_MESSAGE),
      l10n_util::GetStringUTF8(IDS_AUTOFILL_HTTP_WARNING_LEARN_MORE),
      "httpWarning", autofill::POPUP_ITEM_ID_HTTP_NOT_SECURE_WARNING_MESSAGE);
  if (!did_show_form_not_secure_warning_) {
    did_show_form_not_secure_warning_ = true;
    metrics_util::LogShowedFormNotSecureWarningOnCurrentNavigation();
  }
  return http_warning_suggestion;
}
