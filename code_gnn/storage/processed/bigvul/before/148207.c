void CursorImpl::Advance(
    uint32_t count,
    ::indexed_db::mojom::CallbacksAssociatedPtrInfo callbacks_info) {
  scoped_refptr<IndexedDBCallbacks> callbacks(
      new IndexedDBCallbacks(dispatcher_host_->AsWeakPtr(), origin_,
                             std::move(callbacks_info), idb_runner_));
  idb_runner_->PostTask(FROM_HERE, base::Bind(&IDBThreadHelper::Advance,
                                              base::Unretained(helper_), count,
                                              base::Passed(&callbacks)));
}
