void PaymentRequest::RecordDialogShownEventInJourneyLogger() {
  journey_logger_.SetEventOccurred(JourneyLogger::EVENT_SHOWN);
}
