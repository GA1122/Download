void AppCacheDispatcherHost::OnSelectCache(
    int host_id, const GURL& document_url,
    int64 cache_document_was_loaded_from,
    const GURL& opt_manifest_url) {
  if (appcache_service_.get()) {
    if (!backend_impl_.SelectCache(host_id,
                                   document_url,
                                   cache_document_was_loaded_from,
                                   opt_manifest_url)) {
      bad_message::ReceivedBadMessage(this, bad_message::ACDH_SELECT_CACHE);
    }
  } else {
    frontend_proxy_.OnCacheSelected(host_id, AppCacheInfo());
  }
}
