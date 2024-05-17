void CommitUnused(base::WeakPtr<IndexedDBTransaction> transaction) {
  if (!transaction)
    return;
  leveldb::Status status = transaction->Commit();
  DCHECK(status.ok());
}
