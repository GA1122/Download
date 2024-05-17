bool AppCacheBackendImpl::SwapCacheWithCallback(
    int host_id, const SwapCacheCallback& callback, void* callback_param) {
  AppCacheHost* host = GetHost(host_id);
  if (!host)
    return false;

  host->SwapCacheWithCallback(callback, callback_param);
  return true;
}
