void AutoFillManager::FillPhoneNumberField(const AutoFillProfile* profile,
                                           webkit_glue::FormField* field) {
  string16 number = profile->GetFieldText(AutoFillType(PHONE_HOME_NUMBER));
  bool has_valid_suffix_and_prefix = (number.length() ==
      static_cast<size_t>(PhoneNumber::kPrefixLength +
                          PhoneNumber::kSuffixLength));
  if (has_valid_suffix_and_prefix &&
      field->max_length() == PhoneNumber::kPrefixLength) {
    number = number.substr(PhoneNumber::kPrefixOffset,
                           PhoneNumber::kPrefixLength);
    field->set_value(number);
  } else if (has_valid_suffix_and_prefix &&
             field->max_length() == PhoneNumber::kSuffixLength) {
    number = number.substr(PhoneNumber::kSuffixOffset,
                           PhoneNumber::kSuffixLength);
    field->set_value(number);
  } else {
    field->set_value(number);
  }
}
