void PaymentRequestState::NotifyOnGetAllPaymentInstrumentsFinished() {
  for (auto& observer : observers_)
    observer.OnGetAllPaymentInstrumentsFinished();
}
