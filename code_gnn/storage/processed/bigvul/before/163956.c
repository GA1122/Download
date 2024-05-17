void PaymentRequestState::AddAutofillPaymentInstrument(
    bool selected,
    const autofill::CreditCard& card) {
  std::string basic_card_network =
      autofill::data_util::GetPaymentRequestData(card.network())
          .basic_card_issuer_network;
  if (!spec_->supported_card_networks_set().count(basic_card_network) ||
      !spec_->supported_card_types_set().count(card.card_type())) {
    return;
  }

  constexpr size_t kTotalNumberOfCardTypes = 4U;

  bool matches_merchant_card_type_exactly =
      card.card_type() != autofill::CreditCard::CARD_TYPE_UNKNOWN ||
      spec_->supported_card_types_set().size() == kTotalNumberOfCardTypes;

  auto instrument = std::make_unique<AutofillPaymentInstrument>(
      basic_card_network, card, matches_merchant_card_type_exactly,
      shipping_profiles_, app_locale_, payment_request_delegate_);
  available_instruments_.push_back(std::move(instrument));

  if (selected)
    SetSelectedInstrument(available_instruments_.back().get());
}
