VideoCaptureManager::GetControllerSharedRef(
    VideoCaptureController* controller) const {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);

  for (const auto& entry : controllers_) {
    if (entry.get() == controller)
      return entry;
  }
  return nullptr;
}
