void PaymentRequest::DidStartMainFrameNavigationToDifferentDocument(
    bool is_user_initiated) {
  RecordFirstAbortReason(is_user_initiated
                             ? JourneyLogger::ABORT_REASON_USER_NAVIGATION
                             : JourneyLogger::ABORT_REASON_MERCHANT_NAVIGATION);
}
