void PaymentRequestState::SetSelectedShippingProfile(
    autofill::AutofillProfile* profile) {
  spec_->StartWaitingForUpdateWith(
      PaymentRequestSpec::UpdateReason::SHIPPING_ADDRESS);
  selected_shipping_profile_ = profile;

  invalid_shipping_profile_ = nullptr;

  is_waiting_for_merchant_validation_ = true;

  payment_request_delegate_->GetAddressNormalizer()->NormalizeAddressAsync(
      *selected_shipping_profile_,  2,
      base::BindOnce(&PaymentRequestState::OnAddressNormalized,
                     weak_ptr_factory_.GetWeakPtr()));
}
