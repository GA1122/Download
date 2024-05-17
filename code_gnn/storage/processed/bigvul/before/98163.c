string16 CombineLabelAndCreditCard(const string16& label,
                                   const CreditCard* cc) {
  if (label.empty())
    return kCreditCardPrefix + cc->LastFourDigits();
  else if (cc->LastFourDigits().empty())
    return label;
  else
    return label + kLabelSeparator + cc->LastFourDigits();
}
