bool AutofillManager::GetCreditCard(int unique_id,
                                    const CreditCard** credit_card) {
  std::string credit_card_id;
  std::string profile_id;
  SplitFrontendID(unique_id, &credit_card_id, &profile_id);
  *credit_card = nullptr;
  if (base::IsValidGUID(credit_card_id))
    *credit_card = personal_data_->GetCreditCardByGUID(credit_card_id);
  return !!*credit_card;
}
