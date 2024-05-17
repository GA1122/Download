bool AppCacheBackendImpl::StartUpdateWithCallback(
    int host_id, const StartUpdateCallback& callback, void* callback_param) {
  AppCacheHost* host = GetHost(host_id);
  if (!host)
    return false;

  host->StartUpdateWithCallback(callback, callback_param);
  return true;
}
