VideoCaptureManager::LookupControllerByMediaTypeAndDeviceId(
    MediaStreamType type,
    const std::string& device_id) const {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);

  for (const auto& entry : controllers_) {
    if (type == entry->stream_type() && device_id == entry->device_id())
      return entry.get();
  }
  return nullptr;
}
