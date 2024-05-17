void PaymentRequestState::OnAddressNormalized(
    bool success,
    const autofill::AutofillProfile& normalized_profile) {
  delegate_->OnShippingAddressSelected(
      data_util::GetPaymentAddressFromAutofillProfile(normalized_profile,
                                                      app_locale_));
}
