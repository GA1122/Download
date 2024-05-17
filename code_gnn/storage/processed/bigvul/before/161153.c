void MediaStreamManager::RemoveAllVideoCaptureObservers() {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  if (video_capture_manager_) {
    video_capture_manager_->RemoveAllVideoCaptureObservers();
  }
}
