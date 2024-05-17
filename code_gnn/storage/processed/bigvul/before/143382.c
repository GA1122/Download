void Job::OnDnsOperationComplete(int result) {
  CheckIsOnOriginThread();

  DCHECK(!cancelled_.IsSet());
  DCHECK(pending_dns_completed_synchronously_ == (pending_dns_ == NULL));

  SaveDnsToLocalCache(pending_dns_host_, pending_dns_op_, result,
                      pending_dns_addresses_);
  pending_dns_ = NULL;

  if (blocking_dns_) {
    event_.Signal();
    return;
  }

  if (!blocking_dns_ && !pending_dns_completed_synchronously_) {
    worker_task_runner()->PostTask(FROM_HERE,
                                   base::Bind(&Job::ExecuteNonBlocking, this));
  }
}
