void VideoCaptureManager::RemoveAllVideoCaptureObservers() {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  capture_observers_.Clear();
}
