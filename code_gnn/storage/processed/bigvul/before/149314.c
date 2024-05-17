void DatabaseImpl::IDBThreadHelper::RemoveObservers(
    const std::vector<int32_t>& observers) {
  DCHECK(idb_thread_checker_.CalledOnValidThread());
  if (!connection_->IsConnected())
    return;

  connection_->RemoveObservers(observers);
}
