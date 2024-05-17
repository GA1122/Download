  void DoDelete(std::vector<ScopeLock> locks) {
    Status s;
    if (db_->backing_store_)
      s = db_->backing_store_->DeleteDatabase(db_->metadata_.name);
    if (!s.ok()) {
      IndexedDBDatabaseError error(blink::kWebIDBDatabaseExceptionUnknownError,
                                   "Internal error deleting database.");
      callbacks_->OnError(error);
      if (s.IsCorruption()) {
        url::Origin origin = db_->backing_store_->origin();
        db_->backing_store_ = nullptr;
        db_->factory_->HandleBackingStoreCorruption(origin, error);
      }
      db_->RequestComplete(this);
      return;
    }

    int64_t old_version = db_->metadata_.version;
    db_->metadata_.id = kInvalidId;
    db_->metadata_.version = IndexedDBDatabaseMetadata::NO_VERSION;
    db_->metadata_.max_object_store_id = kInvalidId;
    db_->metadata_.object_stores.clear();
    callbacks_->OnSuccess(old_version);
    db_->factory_->DatabaseDeleted(db_->identifier_);

    db_->RequestComplete(this);
  }
