void AppCacheDispatcherHost::OnMarkAsForeignEntry(
    int host_id, const GURL& document_url,
    int64 cache_document_was_loaded_from) {
  if (appcache_service_.get()) {
    if (!backend_impl_.MarkAsForeignEntry(
            host_id, document_url, cache_document_was_loaded_from)) {
      bad_message::ReceivedBadMessage(this,
                                      bad_message::ACDH_MARK_AS_FOREIGN_ENTRY);
    }
  }
}
