base::string16 FieldName(const FormFieldData& field,
                         bool ambiguous_or_empty_names) {
  return ambiguous_or_empty_names
             ? IsPasswordField(field) ? base::ASCIIToUTF16(kDummyPasswordField)
                                      : base::ASCIIToUTF16(kDummyUsernameField)
             : field.name;
}
