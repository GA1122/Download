void VideoCaptureManager::AddVideoCaptureObserver(
    media::VideoCaptureObserver* observer) {
  DCHECK(observer);
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  capture_observers_.AddObserver(observer);
}
