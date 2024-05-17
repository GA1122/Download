  void StartUpgrade(std::vector<ScopeLock> locks) {
    connection_ = db_->CreateConnection(pending_->database_callbacks,
                                        pending_->child_process_id);
    DCHECK_EQ(db_->connections_.count(connection_.get()), 1UL);

    std::vector<int64_t> object_store_ids;

    IndexedDBTransaction* transaction = connection_->CreateTransaction(
        pending_->transaction_id,
        std::set<int64_t>(object_store_ids.begin(), object_store_ids.end()),
        blink::mojom::IDBTransactionMode::VersionChange,
        new IndexedDBBackingStore::Transaction(db_->backing_store()));
    transaction->ScheduleTask(
        base::BindOnce(&IndexedDBDatabase::VersionChangeOperation, db_,
                       pending_->version, pending_->callbacks));
    transaction->Start(std::move(locks));
  }
