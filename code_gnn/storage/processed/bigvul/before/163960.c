bool PaymentRequestState::ArePaymentOptionsSatisfied() {
  if (is_waiting_for_merchant_validation_)
    return false;

  if (!profile_comparator()->IsShippingComplete(selected_shipping_profile_))
    return false;

  if (spec_->request_shipping() && !spec_->selected_shipping_option())
    return false;

  return profile_comparator()->IsContactInfoComplete(selected_contact_profile_);
}
