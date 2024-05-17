void AppCacheDispatcherHost::OnSelectCacheForWorker(
    int host_id, int parent_process_id, int parent_host_id) {
  if (appcache_service_.get()) {
    if (!backend_impl_.SelectCacheForWorker(
            host_id, parent_process_id, parent_host_id)) {
      bad_message::ReceivedBadMessage(
          this, bad_message::ACDH_SELECT_CACHE_FOR_WORKER);
    }
  } else {
    frontend_proxy_.OnCacheSelected(host_id, AppCacheInfo());
  }
}
