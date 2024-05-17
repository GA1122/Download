void CursorImpl::IDBThreadHelper::Prefetch(
    int32_t count,
    scoped_refptr<IndexedDBCallbacks> callbacks) {
  cursor_->PrefetchContinue(count, std::move(callbacks));
}
