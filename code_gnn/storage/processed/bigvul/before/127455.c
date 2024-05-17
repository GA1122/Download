void FileAPIMessageFilter::OnWillUpdate(const GURL& path) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));
  FileSystemURL url(path);
  if (!url.is_valid())
    return;
  const UpdateObserverList* observers =
      context_->GetUpdateObservers(url.type());
  if (!observers)
    return;
  observers->Notify(&FileUpdateObserver::OnStartUpdate, MakeTuple(url));
}
