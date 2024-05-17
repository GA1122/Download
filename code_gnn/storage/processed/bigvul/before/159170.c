void DownloadItemImpl::NotifyRemoved() {
  for (auto& observer : observers_)
    observer.OnDownloadRemoved(this);
}
