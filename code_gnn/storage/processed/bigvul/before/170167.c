bool PasswordAutofillAgent::ShowSuggestions(
    const blink::WebInputElement& element,
    bool show_all,
    bool generation_popup_showing) {
  blink::WebInputElement username_element;
  blink::WebInputElement password_element;
  PasswordInfo* password_info;
  if (!FindPasswordInfoForElement(element, &username_element, &password_element,
                                  &password_info))
    return false;

  if (!element.isTextField() || !IsElementAutocompletable(element) ||
      !IsElementAutocompletable(password_element))
    return true;

  if (element.nameForAutofill().isEmpty() &&
      !DoesFormContainAmbiguousOrEmptyNames(password_info->fill_data)) {
    return false;   
  }

  if (element.value().length() > kMaximumTextSizeForAutocomplete)
    return false;

  if (element.isPasswordField() &&
      (password_info->password_field_suggestion_was_accepted &&
       element != password_info->password_field))
    return true;

  UMA_HISTOGRAM_BOOLEAN(
      "PasswordManager.AutocompletePopupSuppressedByGeneration",
      generation_popup_showing);

  if (generation_popup_showing)
    return false;

  return ShowSuggestionPopup(*password_info, element,
                             show_all && !element.isPasswordField(),
                             element.isPasswordField());
}
