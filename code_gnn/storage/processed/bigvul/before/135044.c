 bool AppCacheBackendImpl::GetStatusWithCallback(
    int host_id, const GetStatusCallback& callback, void* callback_param) {
  AppCacheHost* host = GetHost(host_id);
  if (!host)
    return false;

  host->GetStatusWithCallback(callback, callback_param);
  return true;
}
