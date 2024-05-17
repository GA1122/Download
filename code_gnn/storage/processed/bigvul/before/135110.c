void AppCacheDispatcherHost::OnSelectCacheForSharedWorker(
    int host_id, int64 appcache_id) {
  if (appcache_service_.get()) {
    if (!backend_impl_.SelectCacheForSharedWorker(host_id, appcache_id))
      bad_message::ReceivedBadMessage(
          this, bad_message::ACDH_SELECT_CACHE_FOR_SHARED_WORKER);
  } else {
    frontend_proxy_.OnCacheSelected(host_id, AppCacheInfo());
  }
}
