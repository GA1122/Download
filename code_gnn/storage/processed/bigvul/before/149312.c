void DatabaseImpl::IDBThreadHelper::Put(
    int64_t transaction_id,
    int64_t object_store_id,
    ::indexed_db::mojom::ValuePtr mojo_value,
    std::vector<std::unique_ptr<storage::BlobDataHandle>> handles,
    std::vector<IndexedDBBlobInfo> blob_info,
    const IndexedDBKey& key,
    blink::WebIDBPutMode mode,
    const std::vector<IndexedDBIndexKeys>& index_keys,
    scoped_refptr<IndexedDBCallbacks> callbacks) {
  DCHECK(idb_thread_checker_.CalledOnValidThread());
  if (!connection_->IsConnected())
    return;

  IndexedDBTransaction* transaction =
      connection_->GetTransaction(transaction_id);
  if (!transaction)
    return;

  uint64_t commit_size = mojo_value->bits.size();
  IndexedDBValue value;
  swap(value.bits, mojo_value->bits);
  swap(value.blob_info, blob_info);
  connection_->database()->Put(transaction, object_store_id, &value, &handles,
                               base::MakeUnique<IndexedDBKey>(key), mode,
                               std::move(callbacks), index_keys);

  transaction->set_size(transaction->size() + commit_size);
}
