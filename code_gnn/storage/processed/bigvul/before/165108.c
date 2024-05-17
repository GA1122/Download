void FileSystemOperationRunner::PrepareForWrite(OperationID id,
                                                const FileSystemURL& url) {
  if (file_system_context_->GetUpdateObservers(url.type())) {
    file_system_context_->GetUpdateObservers(url.type())
        ->Notify(&FileUpdateObserver::OnStartUpdate, url);
  }
  write_target_urls_[id].insert(url);
}
