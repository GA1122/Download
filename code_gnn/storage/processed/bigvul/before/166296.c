void VideoCaptureManager::UnregisterListener(
    MediaStreamProviderListener* listener) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  listeners_.RemoveObserver(listener);
}
