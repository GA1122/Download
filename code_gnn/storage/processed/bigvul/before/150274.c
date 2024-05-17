void PasswordAutofillAgent::AutofillField(const base::string16& value,
                                          WebInputElement field) {
  if (field.Value().Utf16() != value)
    field.SetSuggestedValue(WebString::FromUTF16(value));
  field.SetAutofillState(WebAutofillState::kAutofilled);
  gatekeeper_.RegisterElement(&field);
  field_data_manager_.UpdateFieldDataMap(
      field, value, FieldPropertiesFlags::AUTOFILLED_ON_PAGELOAD);
  autofilled_elements_cache_.emplace(field.UniqueRendererFormControlId(),
                                     WebString::FromUTF16(value));
}
