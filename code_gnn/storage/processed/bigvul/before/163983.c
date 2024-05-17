void PaymentRequestState::PopulateProfileCache() {
  std::vector<autofill::AutofillProfile*> profiles =
      personal_data_manager_->GetProfilesToSuggest();

  std::vector<autofill::AutofillProfile*> raw_profiles_for_filtering;
  raw_profiles_for_filtering.reserve(profiles.size());

  for (size_t i = 0; i < profiles.size(); i++) {
    profile_cache_.push_back(
        std::make_unique<autofill::AutofillProfile>(*profiles[i]));
    raw_profiles_for_filtering.push_back(profile_cache_.back().get());
  }

  contact_profiles_ = profile_comparator()->FilterProfilesForContact(
      raw_profiles_for_filtering);
  shipping_profiles_ = profile_comparator()->FilterProfilesForShipping(
      raw_profiles_for_filtering);

  if (spec_->request_payer_name() || spec_->request_payer_phone() ||
      spec_->request_payer_email()) {
    bool has_complete_contact =
        contact_profiles_.empty()
            ? false
            : profile_comparator()->IsContactInfoComplete(contact_profiles_[0]);
    journey_logger_->SetNumberOfSuggestionsShown(
        JourneyLogger::Section::SECTION_CONTACT_INFO, contact_profiles_.size(),
        has_complete_contact);
  }
  if (spec_->request_shipping()) {
    bool has_complete_shipping =
        shipping_profiles_.empty()
            ? false
            : profile_comparator()->IsShippingComplete(shipping_profiles_[0]);
    journey_logger_->SetNumberOfSuggestionsShown(
        JourneyLogger::Section::SECTION_SHIPPING_ADDRESS,
        shipping_profiles_.size(), has_complete_shipping);
  }

  const std::vector<autofill::CreditCard*>& cards =
      personal_data_manager_->GetCreditCardsToSuggest(
           base::FeatureList::IsEnabled(
              payments::features::kReturnGooglePayInBasicCard));
  for (autofill::CreditCard* card : cards)
    AddAutofillPaymentInstrument( false, *card);
}
