std::vector<CreditCard> TestingAutomationProvider::GetCreditCardsFromList(
    const ListValue& cards, std::string* error_message) {
  std::vector<CreditCard> credit_cards;
  DictionaryValue* card_info = NULL;
  string16 current_value;

  std::map<AutofillFieldType, std::string> credit_card_type_to_string =
      GetCreditCardFieldToStringMap();

  int num_credit_cards = cards.GetSize();
  for (int i = 0; i < num_credit_cards; i++) {
    if (!cards.GetDictionary(i, &card_info))
      continue;
    CreditCard card;
    for (std::map<AutofillFieldType, std::string>::iterator type_it =
        credit_card_type_to_string.begin();
        type_it != credit_card_type_to_string.end(); ++type_it) {
      if (card_info->HasKey(type_it->second)) {
        if (card_info->GetString(type_it->second, &current_value)) {
          card.SetInfo(type_it->first, current_value);
        } else {
          *error_message= "All values must be strings";
          break;
        }
      }
    }
    credit_cards.push_back(card);
  }
  return credit_cards;
}
