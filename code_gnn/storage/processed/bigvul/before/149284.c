void DatabaseImpl::Clear(
    int64_t transaction_id,
    int64_t object_store_id,
    ::indexed_db::mojom::CallbacksAssociatedPtrInfo callbacks_info) {
  scoped_refptr<IndexedDBCallbacks> callbacks(
      new IndexedDBCallbacks(dispatcher_host_->AsWeakPtr(), origin_,
                             std::move(callbacks_info), idb_runner_));
  idb_runner_->PostTask(
      FROM_HERE,
      base::Bind(&IDBThreadHelper::Clear, base::Unretained(helper_),
                 transaction_id, object_store_id, base::Passed(&callbacks)));
}
