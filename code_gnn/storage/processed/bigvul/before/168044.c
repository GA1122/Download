AutofillMetrics::CardNumberStatus AutofillManager::GetCardNumberStatus(
    CreditCard& credit_card) {
  base::string16 number = credit_card.number();
  if (number.empty())
    return AutofillMetrics::EMPTY_CARD;
  else if (!HasCorrectLength(number))
    return AutofillMetrics::WRONG_SIZE_CARD;
  else if (!PassesLuhnCheck(number))
    return AutofillMetrics::FAIL_LUHN_CHECK_CARD;
  else if (personal_data_->IsKnownCard(credit_card))
    return AutofillMetrics::KNOWN_CARD;
  else
    return AutofillMetrics::UNKNOWN_CARD;
}
