bool PaymentRequestState::ArePaymentDetailsSatisfied() {
  return selected_instrument_ != nullptr &&
         selected_instrument_->IsCompleteForPayment();
}
