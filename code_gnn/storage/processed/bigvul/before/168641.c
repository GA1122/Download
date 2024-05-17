leveldb::Status IndexedDBTransaction::BlobWriteComplete(
    IndexedDBBackingStore::BlobWriteResult result) {
  IDB_TRACE("IndexedDBTransaction::BlobWriteComplete");
  if (state_ == FINISHED)   
    return leveldb::Status::OK();
  DCHECK_EQ(state_, COMMITTING);

  leveldb::Status s = leveldb::Status::OK();
  switch (result) {
    case IndexedDBBackingStore::BlobWriteResult::FAILURE_ASYNC:
      Abort(IndexedDBDatabaseError(blink::kWebIDBDatabaseExceptionDataError,
                                   "Failed to write blobs."));
      return leveldb::Status::OK();
    case IndexedDBBackingStore::BlobWriteResult::SUCCESS_ASYNC:
    case IndexedDBBackingStore::BlobWriteResult::SUCCESS_SYNC: {
      scoped_refptr<IndexedDBDatabase> database = database_;
      s = CommitPhaseTwo();
      if (!s.ok() &&
          result == IndexedDBBackingStore::BlobWriteResult::SUCCESS_ASYNC)
        database->ReportError(s);
      break;
    }
  }
  return s;
}
