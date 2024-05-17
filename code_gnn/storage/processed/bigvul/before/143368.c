void Job::DoDnsOperation() {
  CheckIsOnOriginThread();
  DCHECK(!pending_dns_);

  if (cancelled_.IsSet())
    return;

  HostResolver::RequestHandle dns_request = NULL;
  int result = host_resolver()->Resolve(
      MakeDnsRequestInfo(pending_dns_host_, pending_dns_op_), DEFAULT_PRIORITY,
      &pending_dns_addresses_, base::Bind(&Job::OnDnsOperationComplete, this),
      &dns_request, bindings_->GetBoundNetLog());

  pending_dns_completed_synchronously_ = result != ERR_IO_PENDING;

  if (cancelled_.IsSet())
    return;

  if (pending_dns_completed_synchronously_) {
    OnDnsOperationComplete(result);
  } else {
    DCHECK(dns_request);
    pending_dns_ = dns_request;
  }

  if (!blocking_dns_) {
    event_.Signal();
  }
}
