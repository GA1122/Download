Status IndexedDBDatabase::PutOperation(
    std::unique_ptr<PutOperationParams> params,
    IndexedDBTransaction* transaction) {
  IDB_TRACE2("IndexedDBDatabase::PutOperation", "txn.id", transaction->id(),
             "size", params->value.SizeEstimate());
  DCHECK_NE(transaction->mode(), blink::mojom::IDBTransactionMode::ReadOnly);
  bool key_was_generated = false;
  Status s = Status::OK();

  DCHECK(metadata_.object_stores.find(params->object_store_id) !=
         metadata_.object_stores.end());
  const IndexedDBObjectStoreMetadata& object_store =
      metadata_.object_stores[params->object_store_id];
  DCHECK(object_store.auto_increment || params->key->IsValid());

  std::unique_ptr<IndexedDBKey> key;
  if (params->put_mode != blink::mojom::IDBPutMode::CursorUpdate &&
      object_store.auto_increment && !params->key->IsValid()) {
    std::unique_ptr<IndexedDBKey> auto_inc_key = GenerateKey(
        backing_store_.get(), transaction, id(), params->object_store_id);
    key_was_generated = true;
    if (!auto_inc_key->IsValid()) {
      params->callbacks->OnError(
          CreateError(blink::kWebIDBDatabaseExceptionConstraintError,
                      "Maximum key generator value reached.", transaction));
      return s;
    }
    key = std::move(auto_inc_key);
  } else {
    key = std::move(params->key);
  }

  DCHECK(key->IsValid());

  IndexedDBBackingStore::RecordIdentifier record_identifier;
  if (params->put_mode == blink::mojom::IDBPutMode::AddOnly) {
    bool found = false;
    Status found_status = backing_store_->KeyExistsInObjectStore(
        transaction->BackingStoreTransaction(), id(), params->object_store_id,
        *key, &record_identifier, &found);
    if (!found_status.ok())
      return found_status;
    if (found) {
      params->callbacks->OnError(
          CreateError(blink::kWebIDBDatabaseExceptionConstraintError,
                      "Key already exists in the object store.", transaction));
      return found_status;
    }
  }

  std::vector<std::unique_ptr<IndexWriter>> index_writers;
  base::string16 error_message;
  bool obeys_constraints = false;
  bool backing_store_success = MakeIndexWriters(transaction,
                                                backing_store_.get(),
                                                id(),
                                                object_store,
                                                *key,
                                                key_was_generated,
                                                params->index_keys,
                                                &index_writers,
                                                &error_message,
                                                &obeys_constraints);
  if (!backing_store_success) {
    params->callbacks->OnError(
        CreateError(blink::kWebIDBDatabaseExceptionUnknownError,
                    "Internal error: backing store error updating index keys.",
                    transaction));
    return s;
  }
  if (!obeys_constraints) {
    params->callbacks->OnError(
        CreateError(blink::kWebIDBDatabaseExceptionConstraintError,
                    error_message, transaction));
    return s;
  }

  s = backing_store_->PutRecord(transaction->BackingStoreTransaction(), id(),
                                params->object_store_id, *key, &params->value,
                                &record_identifier);
  if (!s.ok())
    return s;

  {
    IDB_TRACE1("IndexedDBDatabase::PutOperation.UpdateIndexes", "txn.id",
               transaction->id());
    for (const auto& writer : index_writers) {
      writer->WriteIndexKeys(record_identifier, backing_store_.get(),
                             transaction->BackingStoreTransaction(), id(),
                             params->object_store_id);
    }
  }

  if (object_store.auto_increment &&
      params->put_mode != blink::mojom::IDBPutMode::CursorUpdate &&
      key->type() == blink::mojom::IDBKeyType::Number) {
    IDB_TRACE1("IndexedDBDatabase::PutOperation.AutoIncrement", "txn.id",
               transaction->id());
    s = UpdateKeyGenerator(backing_store_.get(), transaction, id(),
                           params->object_store_id, *key, !key_was_generated);
    if (!s.ok())
      return s;
  }
  {
    IDB_TRACE1("IndexedDBDatabase::PutOperation.Callbacks", "txn.id",
               transaction->id());
    params->callbacks->OnSuccess(*key);
  }
  FilterObservation(transaction, params->object_store_id,
                    params->put_mode == blink::mojom::IDBPutMode::AddOnly
                        ? blink::mojom::IDBOperationType::Add
                        : blink::mojom::IDBOperationType::Put,
                    IndexedDBKeyRange(*key), &params->value);
  factory_->NotifyIndexedDBContentChanged(
      origin(), metadata_.name,
      metadata_.object_stores[params->object_store_id].name);
  return s;
}
