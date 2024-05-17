void CursorImpl::IDBThreadHelper::Continue(
    const IndexedDBKey& key,
    const IndexedDBKey& primary_key,
    scoped_refptr<IndexedDBCallbacks> callbacks) {
  cursor_->Continue(
      key.IsValid() ? base::MakeUnique<IndexedDBKey>(key) : nullptr,
      primary_key.IsValid() ? base::MakeUnique<IndexedDBKey>(primary_key)
                            : nullptr,
      std::move(callbacks));
}
