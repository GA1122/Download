ListValue* TestingAutomationProvider::GetListFromCreditCards(
    const std::vector<CreditCard*>& credit_cards) {
  ListValue* cards = new ListValue;

  std::map<AutofillFieldType, std::string> credit_card_type_to_string =
      GetCreditCardFieldToStringMap();

  for (std::vector<CreditCard*>::const_iterator it =
           credit_cards.begin();
       it != credit_cards.end(); ++it) {
    CreditCard* card = *it;
    DictionaryValue* card_info = new DictionaryValue;
    for (std::map<AutofillFieldType, std::string>::iterator type_it =
        credit_card_type_to_string.begin();
        type_it != credit_card_type_to_string.end(); ++type_it) {
      string16 value = card->GetInfo(type_it->first);
      if (value.length()) {
        card_info->SetString(type_it->second, value);
      }
    }
    cards->Append(card_info);
  }
  return cards;
}
