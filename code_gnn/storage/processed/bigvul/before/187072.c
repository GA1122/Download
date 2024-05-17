 void PaymentRequest::UpdateWith(mojom::PaymentDetailsPtr details) {
    std::string error;
    if (!ValidatePaymentDetails(ConvertPaymentDetails(details), &error)) {
    LOG(ERROR) << error;
//     log_.Error(error);
      OnConnectionTerminated();
      return;
    }
  
    if (details->shipping_address_errors &&
        !PaymentsValidators::IsValidAddressErrorsFormat(
            details->shipping_address_errors, &error)) {
    DLOG(ERROR) << error;
//     log_.Error(error);
      OnConnectionTerminated();
      return;
    }
  
    if (!details->total) {
    LOG(ERROR) << "Missing total";
//     log_.Error("Missing total");
      OnConnectionTerminated();
      return;
    }
 
   spec_->UpdateWith(std::move(details));
  }