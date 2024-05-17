void PaymentRequestState::CheckRequestedMethodsSupported(
    StatusCallback callback) {
  DCHECK(get_all_instruments_finished_);

  std::move(callback).Run(are_requested_methods_supported_);
}
