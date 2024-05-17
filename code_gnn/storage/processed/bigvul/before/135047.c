bool AppCacheBackendImpl::SetSpawningHostId(
    int host_id,
    int spawning_host_id) {
  AppCacheHost* host = GetHost(host_id);
  if (!host)
    return false;
  host->SetSpawningHostId(process_id_, spawning_host_id);
  return true;
}
