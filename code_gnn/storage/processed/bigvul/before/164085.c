void AppCache::UnassociateHost(AppCacheHost* host) {
  associated_hosts_.erase(host);
}
