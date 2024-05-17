void Job::HandleAlertOrError(bool is_alert,
                             int line_number,
                             const base::string16& message) {
  CheckIsOnWorkerThread();

  if (cancelled_.IsSet())
    return;

  if (blocking_dns_) {
    origin_runner_->PostTask(
        FROM_HERE, base::Bind(&Job::DispatchAlertOrErrorOnOriginThread, this,
                              is_alert, line_number, message));
    return;
  }


  if (abandoned_)
    return;

  alerts_and_errors_byte_cost_ += sizeof(AlertOrError) + message.size() * 2;

  if (alerts_and_errors_byte_cost_ > kMaxAlertsAndErrorsBytes) {
    alerts_and_errors_.clear();
    ScheduleRestartWithBlockingDns();
    return;
  }

  AlertOrError entry = {is_alert, line_number, message};
  alerts_and_errors_.push_back(entry);
}
