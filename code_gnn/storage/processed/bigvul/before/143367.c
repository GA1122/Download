void Job::DispatchBufferedAlertsAndErrors() {
  CheckIsOnOriginThread();
  for (size_t i = 0; i < alerts_and_errors_.size(); ++i) {
    const AlertOrError& x = alerts_and_errors_[i];
    DispatchAlertOrErrorOnOriginThread(x.is_alert, x.line_number, x.message);
  }
}
