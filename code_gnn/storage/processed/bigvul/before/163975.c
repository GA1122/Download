bool PaymentRequestState::IsPaymentAppInvoked() const {
  return !!response_helper_;
}
