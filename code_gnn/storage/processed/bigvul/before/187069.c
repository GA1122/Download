  void PaymentRequest::Retry(mojom::PaymentValidationErrorsPtr errors) {
  if (!client_.is_bound() || !binding_.is_bound()) {
    DLOG(ERROR) << "Attempted Retry(), but binding(s) missing.";
//   if (!IsInitialized()) {
//     log_.Error("Attempted retry without initialization");
      OnConnectionTerminated();
      return;
    }
  
  if (!display_handle_) {
    DLOG(ERROR) << "Attempted Retry(), but display_handle_ is nullptr.";
//   if (!IsThisPaymentRequestShowing()) {
//     log_.Error("Attempted retry without show");
      OnConnectionTerminated();
      return;
    }
  
    std::string error;
    if (!PaymentsValidators::IsValidPaymentValidationErrorsFormat(errors,
                                                                  &error)) {
    DLOG(ERROR) << error;
//     log_.Error(error);
      client_->OnError(mojom::PaymentErrorReason::USER_CANCEL);
      OnConnectionTerminated();
      return;
   }
 
   spec()->Retry(std::move(errors));
    display_handle_->Retry();
  }