void PaymentRequestState::CheckCanMakePayment(bool legacy_mode,
                                              StatusCallback callback) {
  DCHECK(get_all_instruments_finished_);
  if (!legacy_mode) {
    std::move(callback).Run(are_requested_methods_supported_);
    return;
  }

  bool can_make_payment_value = false;
  for (const auto& instrument : available_instruments_) {
    if (instrument->IsValidForCanMakePayment()) {
      can_make_payment_value = true;
      break;
    }
  }
  std::move(callback).Run(can_make_payment_value);
}
