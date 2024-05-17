VideoCaptureManager* MediaStreamManager::video_capture_manager() {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  DCHECK(video_capture_manager_.get());
  return video_capture_manager_.get();
}
