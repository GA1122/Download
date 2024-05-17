void Job::ScheduleRestartWithBlockingDns() {
  CheckIsOnWorkerThread();

  DCHECK(!should_restart_with_blocking_dns_);
  DCHECK(!abandoned_);
  DCHECK(!blocking_dns_);

  abandoned_ = true;

  should_restart_with_blocking_dns_ = true;
}
