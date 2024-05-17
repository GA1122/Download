LoadState Job::GetLoadState() const {
  CheckIsOnOriginThread();

  if (pending_dns_)
    return LOAD_STATE_RESOLVING_HOST_IN_PROXY_SCRIPT;

  return LOAD_STATE_RESOLVING_PROXY_FOR_URL;
}
