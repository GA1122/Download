void PaymentRequestState::GeneratePaymentResponse() {
  DCHECK(is_ready_to_pay());

  response_helper_ = std::make_unique<PaymentResponseHelper>(
      app_locale_, spec_, selected_instrument_, payment_request_delegate_,
       selected_shipping_profile_, selected_contact_profile_, this);
 }
