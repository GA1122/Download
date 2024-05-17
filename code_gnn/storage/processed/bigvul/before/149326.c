DatabaseImpl::IDBThreadHelper::~IDBThreadHelper() {
  DCHECK(idb_thread_checker_.CalledOnValidThread());
  if (connection_->IsConnected())
    connection_->Close();
  indexed_db_context_->ConnectionClosed(origin_, connection_.get());
}
