DatabaseImpl::IDBThreadHelper::IDBThreadHelper(
    std::unique_ptr<IndexedDBConnection> connection,
    const url::Origin& origin,
    scoped_refptr<IndexedDBContextImpl> indexed_db_context)
    : indexed_db_context_(indexed_db_context),
      connection_(std::move(connection)),
      origin_(origin),
      weak_factory_(this) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  idb_thread_checker_.DetachFromThread();
}
