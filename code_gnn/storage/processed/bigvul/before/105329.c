void AutofillManager::FillFormField(const AutofillProfile* profile,
                                    AutofillFieldType type,
                                    size_t variant,
                                    webkit_glue::FormField* field) {
  DCHECK(profile);
  DCHECK_NE(AutofillType::CREDIT_CARD, AutofillType(type).group());
  DCHECK(field);

  if (AutofillType(type).subgroup() == AutofillType::PHONE_NUMBER) {
    FillPhoneNumberField(profile, type, variant, field);
  } else {
    if (field->form_control_type == ASCIIToUTF16("select-one")) {
      autofill::FillSelectControl(*profile, type, field);
    } else {
      std::vector<string16> values;
      profile->GetMultiInfo(type, &values);
      NormalizePhoneMultiInfo(type, profile->CountryCode(), &values);
      DCHECK(variant < values.size());
      field->value = values[variant];
    }
  }
}
