void Job::NotifyCallerOnOriginLoop(int result) {
  CheckIsOnOriginThread();

  if (cancelled_.IsSet())
    return;

  DispatchBufferedAlertsAndErrors();

  if (cancelled_.IsSet())
    return;

  DCHECK(!callback_.is_null());
  DCHECK(!pending_dns_);

  if (operation_ == GET_PROXY_FOR_URL) {
    *user_results_ = results_;
  }

  CompletionCallback callback = callback_;
  ReleaseCallback();
  callback.Run(result);

  bindings_.reset();
  owned_self_reference_ = NULL;
}
