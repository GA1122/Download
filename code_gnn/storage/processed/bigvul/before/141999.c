MATCHER_P(CreditCardMatches, card, "") {
  return !arg.Compare(card);
}
