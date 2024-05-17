bool AutofillManager::FormHasAddressField(const FormData& form) {
  for (const FormFieldData& field : form.fields) {
    const AutofillField* autofill_field = GetAutofillField(form, field);
    if (autofill_field && (autofill_field->Type().group() == ADDRESS_HOME ||
                           autofill_field->Type().group() == ADDRESS_BILLING)) {
      return true;
    }
  }

  return false;
}
