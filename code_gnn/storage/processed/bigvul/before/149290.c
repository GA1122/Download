void DatabaseImpl::IDBThreadHelper::ConnectionOpened() {
  DCHECK(idb_thread_checker_.CalledOnValidThread());
  indexed_db_context_->ConnectionOpened(origin_, connection_.get());
}
