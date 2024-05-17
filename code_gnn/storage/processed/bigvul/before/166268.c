bool VideoCaptureManager::IsControllerPointerValid(
    const VideoCaptureController* controller) const {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  return base::ContainsValue(controllers_, controller);
}
