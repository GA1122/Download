void PaymentRequestState::FinishedGetAllSWPaymentInstruments() {
  PopulateProfileCache();
  SetDefaultProfileSelections();

  get_all_instruments_finished_ = true;
  are_requested_methods_supported_ |= !available_instruments_.empty();
  NotifyOnGetAllPaymentInstrumentsFinished();
  NotifyInitialized();

  if (can_make_payment_callback_) {
    CheckCanMakePayment(can_make_payment_legacy_mode_,
                        std::move(can_make_payment_callback_));
  }

  if (has_enrolled_instrument_callback_)
    CheckHasEnrolledInstrument(std::move(has_enrolled_instrument_callback_));

  if (are_requested_methods_supported_callback_)
    CheckRequestedMethodsSupported(
        std::move(are_requested_methods_supported_callback_));
}
