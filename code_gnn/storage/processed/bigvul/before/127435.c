void FileAPIMessageFilter::OnChannelClosing() {
  BrowserMessageFilter::OnChannelClosing();

  for (base::hash_set<std::string>::const_iterator iter = blob_urls_.begin();
       iter != blob_urls_.end(); ++iter) {
    blob_storage_context_->controller()->RemoveBlob(GURL(*iter));
  }

  if (!open_filesystem_urls_.empty()) {
    DLOG(INFO)
        << "File API: Renderer process shut down before NotifyCloseFile"
        << " for " << open_filesystem_urls_.size() << " files opened in PPAPI";
  }
  for (std::multiset<GURL>::const_iterator iter =
       open_filesystem_urls_.begin();
       iter != open_filesystem_urls_.end(); ++iter) {
    FileSystemURL url(*iter);
    FileSystemOperation* operation = context_->CreateFileSystemOperation(
        url, NULL);
    if (operation)
      operation->NotifyCloseFile(url);
  }
}
