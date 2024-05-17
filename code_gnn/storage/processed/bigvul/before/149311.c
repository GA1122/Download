void DatabaseImpl::Put(
    int64_t transaction_id,
    int64_t object_store_id,
    ::indexed_db::mojom::ValuePtr value,
    const IndexedDBKey& key,
    blink::WebIDBPutMode mode,
    const std::vector<IndexedDBIndexKeys>& index_keys,
    ::indexed_db::mojom::CallbacksAssociatedPtrInfo callbacks_info) {
  ChildProcessSecurityPolicyImpl* policy =
      ChildProcessSecurityPolicyImpl::GetInstance();

  scoped_refptr<IndexedDBCallbacks> callbacks(
      new IndexedDBCallbacks(dispatcher_host_->AsWeakPtr(), origin_,
                             std::move(callbacks_info), idb_runner_));

  std::vector<std::unique_ptr<storage::BlobDataHandle>> handles(
      value->blob_or_file_info.size());
  std::vector<IndexedDBBlobInfo> blob_info(value->blob_or_file_info.size());
  for (size_t i = 0; i < value->blob_or_file_info.size(); ++i) {
    ::indexed_db::mojom::BlobInfoPtr& info = value->blob_or_file_info[i];

    std::unique_ptr<storage::BlobDataHandle> handle =
        dispatcher_host_->blob_storage_context()->GetBlobDataFromUUID(
            info->uuid);

    UMA_HISTOGRAM_BOOLEAN("Storage.IndexedDB.PutValidBlob",
                          handle.get() != nullptr);
    if (!handle) {
      IndexedDBDatabaseError error(blink::kWebIDBDatabaseExceptionUnknownError,
                                   kInvalidBlobUuid);
      idb_runner_->PostTask(
          FROM_HERE, base::Bind(&IDBThreadHelper::AbortWithError,
                                base::Unretained(helper_), transaction_id,
                                base::Passed(&callbacks), error));
      return;
    }
    UMA_HISTOGRAM_MEMORY_KB("Storage.IndexedDB.PutBlobSizeKB",
                            handle->size() / 1024ull);

    handles[i] = std::move(handle);

    if (info->file) {
      if (!info->file->path.empty() &&
          !policy->CanReadFile(dispatcher_host_->ipc_process_id(),
                               info->file->path)) {
        mojo::ReportBadMessage(kInvalidBlobFilePath);
        return;
      }
      blob_info[i] = IndexedDBBlobInfo(info->uuid, info->file->path,
                                       info->file->name, info->mime_type);
      if (info->size != -1) {
        blob_info[i].set_last_modified(info->file->last_modified);
        blob_info[i].set_size(info->size);
      }
    } else {
      blob_info[i] = IndexedDBBlobInfo(info->uuid, info->mime_type, info->size);
    }
  }

  idb_runner_->PostTask(
      FROM_HERE,
      base::Bind(&IDBThreadHelper::Put, base::Unretained(helper_),
                 transaction_id, object_store_id, base::Passed(&value),
                 base::Passed(&handles), base::Passed(&blob_info), key, mode,
                 index_keys, base::Passed(&callbacks)));
}
