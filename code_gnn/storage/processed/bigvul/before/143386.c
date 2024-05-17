bool Job::PostDnsOperationAndWait(const std::string& host,
                                  ResolveDnsOperation op,
                                  bool* completed_synchronously) {
  DCHECK(!pending_dns_);
  pending_dns_host_ = host;
  pending_dns_op_ = op;
  origin_runner_->PostTask(FROM_HERE, base::Bind(&Job::DoDnsOperation, this));

  event_.Wait();
  event_.Reset();

  if (cancelled_.IsSet())
    return false;

  if (completed_synchronously)
    *completed_synchronously = pending_dns_completed_synchronously_;

  return true;
}
