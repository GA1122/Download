void PasswordAutofillAgent::FillField(WebInputElement* input,
                                      const base::string16& credential) {
  DCHECK(input);
  DCHECK(!input->IsNull());
  input->SetAutofillValue(WebString::FromUTF16(credential));
  input->SetAutofillState(WebAutofillState::kAutofilled);
  field_data_manager_.UpdateFieldDataMap(
      *input, credential, FieldPropertiesFlags::AUTOFILLED_ON_USER_TRIGGER);
}
