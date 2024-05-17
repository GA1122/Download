void DatabaseImpl::IDBThreadHelper::VersionChangeIgnored() {
  DCHECK(idb_thread_checker_.CalledOnValidThread());
  if (!connection_->IsConnected())
    return;

  connection_->VersionChangeIgnored();
}
