bool SaveCardBubbleControllerImpl::InputCvcIsValid(
    const base::string16& input_text) const {
  base::string16 trimmed_text;
  base::TrimWhitespace(input_text, base::TRIM_ALL, &trimmed_text);
  return IsValidCreditCardSecurityCode(trimmed_text, card_.network());
}
