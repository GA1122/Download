void PasswordAutofillManager::DidSelectSuggestion(const base::string16& value,
                                                  int identifier) {
  ClearPreviewedForm();
  if (identifier == autofill::POPUP_ITEM_ID_HTTP_NOT_SECURE_WARNING_MESSAGE ||
      identifier == autofill::POPUP_ITEM_ID_ALL_SAVED_PASSWORDS_ENTRY ||
      identifier == autofill::POPUP_ITEM_ID_GENERATE_PASSWORD_ENTRY)
    return;
  bool success =
      PreviewSuggestion(form_data_key_, GetUsernameFromSuggestion(value));
  DCHECK(success);
}
