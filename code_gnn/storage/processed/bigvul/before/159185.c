void DownloadItemImpl::RemoveObserver(Observer* observer) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);

  observers_.RemoveObserver(observer);
}
