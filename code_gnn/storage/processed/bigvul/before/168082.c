bool AutofillManager::RemoveAutofillProfileOrCreditCard(int unique_id) {
  std::string guid;
  const CreditCard* credit_card = nullptr;
  const AutofillProfile* profile = nullptr;
  if (GetCreditCard(unique_id, &credit_card)) {
    if (credit_card->record_type() != CreditCard::LOCAL_CARD)
      return false;

    guid = credit_card->guid();
  } else if (GetProfile(unique_id, &profile)) {
    if (profile->record_type() != AutofillProfile::LOCAL_PROFILE)
      return false;

    guid = profile->guid();
  } else {
    NOTREACHED();
    return false;
  }

  personal_data_->RemoveByGUID(guid);
  return true;
}
