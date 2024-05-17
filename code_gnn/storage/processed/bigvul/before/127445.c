void FileAPIMessageFilter::OnNotifyCloseFile(const GURL& path) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));

  std::multiset<GURL>::iterator iter = open_filesystem_urls_.find(path);
  DCHECK(iter != open_filesystem_urls_.end());
  open_filesystem_urls_.erase(iter);

  FileSystemURL url(path);

  FileSystemOperation* operation = context_->CreateFileSystemOperation(
      url, NULL);
  if (operation)
    operation->NotifyCloseFile(url);
}
