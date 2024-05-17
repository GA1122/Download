MediaDevicesManager* MediaStreamManager::media_devices_manager() {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  return media_devices_manager_.get();
}
