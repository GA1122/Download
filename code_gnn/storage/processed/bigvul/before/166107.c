void PaymentRequest::Pay() {
  journey_logger_.SetEventOccurred(JourneyLogger::EVENT_PAY_CLICKED);

  DCHECK(state_->selected_instrument());
  JourneyLogger::Event selected_event =
      JourneyLogger::Event::EVENT_SELECTED_OTHER;
  switch (state_->selected_instrument()->type()) {
    case PaymentInstrument::Type::AUTOFILL:
      selected_event = JourneyLogger::Event::EVENT_SELECTED_CREDIT_CARD;
      break;
    case PaymentInstrument::Type::SERVICE_WORKER_APP:
      selected_event = JourneyLogger::Event::EVENT_SELECTED_OTHER;
      break;
    case PaymentInstrument::Type::NATIVE_MOBILE_APP:
      NOTREACHED();
      break;
  }
  journey_logger_.SetEventOccurred(selected_event);

  state_->GeneratePaymentResponse();
}
