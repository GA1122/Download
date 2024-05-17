void PaymentRequest::UserCancelled() {
  if (!client_.is_bound())
    return;

  RecordFirstAbortReason(JourneyLogger::ABORT_REASON_ABORTED_BY_USER);

  client_->OnError(mojom::PaymentErrorReason::USER_CANCEL);

  client_.reset();
  binding_.Close();
  if (observer_for_testing_)
    observer_for_testing_->OnConnectionTerminated();
  manager_->DestroyRequest(this);
}
