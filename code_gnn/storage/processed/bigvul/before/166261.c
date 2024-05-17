VideoCaptureManager::GetDeviceFormatInUse(MediaStreamType stream_type,
                                          const std::string& device_id) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  VideoCaptureController* device_in_use =
      LookupControllerByMediaTypeAndDeviceId(stream_type, device_id);
  return device_in_use ? device_in_use->GetVideoCaptureFormat() : base::nullopt;
}
