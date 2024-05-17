void AutofillManager::GetCreditCardSuggestions(FormStructure* form,
                                               const FormField& field,
                                               AutofillFieldType type,
                                               std::vector<string16>* values,
                                               std::vector<string16>* labels,
                                               std::vector<string16>* icons,
                                               std::vector<int>* unique_ids) {
  for (std::vector<CreditCard*>::const_iterator iter =
           personal_data_->credit_cards().begin();
       iter != personal_data_->credit_cards().end(); ++iter) {
    CreditCard* credit_card = *iter;

    string16 creditcard_field_value = credit_card->GetInfo(type);
    if (!creditcard_field_value.empty() &&
        StartsWith(creditcard_field_value, field.value, false)) {
      if (type == CREDIT_CARD_NUMBER)
        creditcard_field_value = credit_card->ObfuscatedNumber();

      string16 label;
      if (credit_card->number().empty()) {
        label = credit_card->GetInfo(CREDIT_CARD_NAME);
      } else {
        label = kCreditCardPrefix;
        label.append(credit_card->LastFourDigits());
      }

      values->push_back(creditcard_field_value);
      labels->push_back(label);
      icons->push_back(UTF8ToUTF16(credit_card->type()));
      unique_ids->push_back(PackGUIDs(GUIDPair(credit_card->guid(), 0),
                                      GUIDPair(std::string(), 0)));
    }
  }
}
