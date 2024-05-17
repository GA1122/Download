void AutofillManager::FillPhoneNumberField(const AutofillProfile* profile,
                                           AutofillFieldType type,
                                           size_t variant,
                                           webkit_glue::FormField* field) {
  std::vector<string16> values;
  profile->GetMultiInfo(type, &values);
  NormalizePhoneMultiInfo(type, profile->CountryCode(), &values);
  DCHECK(variant < values.size());
  string16 number = values[variant];
  bool has_valid_suffix_and_prefix = (number.length() ==
      static_cast<size_t>(PhoneNumber::kPrefixLength +
                          PhoneNumber::kSuffixLength));
  if (has_valid_suffix_and_prefix &&
      field->max_length == PhoneNumber::kPrefixLength) {
    number = number.substr(PhoneNumber::kPrefixOffset,
                           PhoneNumber::kPrefixLength);
    field->value = number;
  } else if (has_valid_suffix_and_prefix &&
             field->max_length == PhoneNumber::kSuffixLength) {
    number = number.substr(PhoneNumber::kSuffixOffset,
                           PhoneNumber::kSuffixLength);
    field->value = number;
  } else {
    field->value = number;
  }
}
