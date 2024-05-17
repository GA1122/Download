void MediaStreamManager::AddVideoCaptureObserver(
    media::VideoCaptureObserver* capture_observer) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  if (video_capture_manager_) {
    video_capture_manager_->AddVideoCaptureObserver(capture_observer);
  }
}
