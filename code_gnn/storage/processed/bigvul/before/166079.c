void FileSystemManagerImpl::OnConnectionError() {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  if (bindings_.empty()) {
    in_transit_snapshot_files_.Clear();
    operation_runner_.reset();
    cancellable_operations_.CloseAllBindings();
  }
}
