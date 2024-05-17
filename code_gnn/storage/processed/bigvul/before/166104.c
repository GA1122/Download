 void PaymentRequest::OnPaymentResponseAvailable(
     mojom::PaymentResponsePtr response) {
   journey_logger_.SetEventOccurred(
      JourneyLogger::EVENT_RECEIVED_INSTRUMENT_DETAILS);

  if (response->method_name.empty() || response->stringified_details.empty()) {
    RecordFirstAbortReason(
        JourneyLogger::ABORT_REASON_INSTRUMENT_DETAILS_ERROR);
    delegate_->ShowErrorMessage();
    return;
  }

  client_->OnPaymentResponse(std::move(response));
}
