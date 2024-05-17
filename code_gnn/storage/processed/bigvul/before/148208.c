 void CursorImpl::IDBThreadHelper::Advance(
     uint32_t count,
    scoped_refptr<IndexedDBCallbacks> callbacks) {
  cursor_->Advance(count, std::move(callbacks));
}
