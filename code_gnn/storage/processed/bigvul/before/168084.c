 base::string16 SanitizeCreditCardFieldValue(const base::string16& value) {
  base::string16 sanitized;
  base::TrimWhitespace(value, base::TRIM_ALL, &sanitized);
  base::TrimString(sanitized,
                   base::string16({base::i18n::kRightToLeftMark,
                                   base::i18n::kLeftToRightMark}),
                   &sanitized);
  base::RemoveChars(sanitized, base::ASCIIToUTF16("-_"), &sanitized);
  return sanitized;
}
