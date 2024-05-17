void Job::ExecuteBlocking() {
  CheckIsOnWorkerThread();
  DCHECK(blocking_dns_);

  if (cancelled_.IsSet())
    return;

  NotifyCaller(ExecuteProxyResolver());
}
