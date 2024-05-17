void Job::Cancel() {
  CheckIsOnOriginThread();

  cancelled_.Set();

  ReleaseCallback();

  if (pending_dns_) {
    host_resolver()->CancelRequest(pending_dns_);
    pending_dns_ = NULL;
  }

  event_.Signal();

  bindings_.reset();
  owned_self_reference_ = NULL;
}
