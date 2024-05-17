void AutoFillManager::GetCreditCardSuggestions(FormStructure* form,
                                               const FormField& field,
                                               AutoFillType type,
                                               std::vector<string16>* values,
                                               std::vector<string16>* labels,
                                               std::vector<string16>* icons,
                                               std::vector<int>* unique_ids) {
  if (!FormIsHTTPS(form))
    return;

  for (std::vector<CreditCard*>::const_iterator iter =
           personal_data_->credit_cards().begin();
       iter != personal_data_->credit_cards().end(); ++iter) {
    CreditCard* credit_card = *iter;

    string16 creditcard_field_value = credit_card->GetFieldText(type);
    if (!creditcard_field_value.empty() &&
        StartsWith(creditcard_field_value, field.value(), false)) {
      if (type.field_type() == CREDIT_CARD_NUMBER)
        creditcard_field_value = credit_card->ObfuscatedNumber();

      if (!form->HasNonBillingFields()) {
        values->push_back(creditcard_field_value);
        labels->push_back(CombineLabelAndCreditCard(string16(), credit_card));
        icons->push_back(credit_card->type());
        unique_ids->push_back(PackIDs(credit_card->unique_id(), 0));
      } else {
        const std::vector<AutoFillProfile*>& profiles
            = personal_data_->profiles();
        std::vector<string16> inferred_labels;
        AutoFillProfile::CreateInferredLabels(&profiles,
                                              &inferred_labels,
                                              0,
                                              type.field_type());
        DCHECK_EQ(profiles.size(), inferred_labels.size());

        for (size_t i = 0; i < profiles.size(); ++i) {
          values->push_back(creditcard_field_value);

          string16 label = CombineLabelAndCreditCard(inferred_labels[i],
                                                     credit_card);
          labels->push_back(label);
          icons->push_back(credit_card->type());
          unique_ids->push_back(
              PackIDs(credit_card->unique_id(), profiles[i]->unique_id()));
        }
      }
    }
  }
}
