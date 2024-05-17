bool AppCacheBackendImpl::RegisterHost(int id) {
  if (GetHost(id))
    return false;

  hosts_.insert(
      HostMap::value_type(id, new AppCacheHost(id, frontend_, service_)));
  return true;
}
