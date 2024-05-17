void FileAPIMessageFilter::OnDidUpdate(const GURL& path, int64 delta) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));
  FileSystemURL url(path);
  if (!url.is_valid())
    return;
  const UpdateObserverList* observers =
      context_->GetUpdateObservers(url.type());
  if (!observers)
    return;
  observers->Notify(&FileUpdateObserver::OnUpdate, MakeTuple(url, delta));
  observers->Notify(&FileUpdateObserver::OnEndUpdate, MakeTuple(url));
}
