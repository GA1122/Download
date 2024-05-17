void NormalizePhoneMultiInfo(AutofillFieldType type,
                             std::string const& locale,
                             std::vector<string16>* values) {
  DCHECK(values);
  if (type != PHONE_HOME_WHOLE_NUMBER && type != PHONE_FAX_WHOLE_NUMBER)
    return;
  for (std::vector<string16>::iterator it = values->begin();
       it != values->end();
       ++it) {
    string16 normalized_phone = autofill_i18n::NormalizePhoneNumber(*it,
                                                                    locale);
    if (!normalized_phone.empty())
      *it = normalized_phone;
  }
}
