void PaymentRequest::RespondToCanMakePaymentQuery(bool can_make_payment,
                                                  bool warn_localhost_or_file) {
  mojom::CanMakePaymentQueryResult positive =
      warn_localhost_or_file
          ? mojom::CanMakePaymentQueryResult::WARNING_CAN_MAKE_PAYMENT
          : mojom::CanMakePaymentQueryResult::CAN_MAKE_PAYMENT;
  mojom::CanMakePaymentQueryResult negative =
      warn_localhost_or_file
          ? mojom::CanMakePaymentQueryResult::WARNING_CANNOT_MAKE_PAYMENT
          : mojom::CanMakePaymentQueryResult::CANNOT_MAKE_PAYMENT;

  client_->OnCanMakePayment(can_make_payment ? positive : negative);
  journey_logger_.SetCanMakePaymentValue(can_make_payment);
}
