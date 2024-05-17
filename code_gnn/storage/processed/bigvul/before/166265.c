bool VideoCaptureManager::GetDeviceSupportedFormats(
    const std::string& device_id,
    media::VideoCaptureFormats* supported_formats) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  DCHECK(supported_formats->empty());

  media::VideoCaptureDeviceInfo* existing_device = GetDeviceInfoById(device_id);
  if (existing_device)
    *supported_formats = existing_device->supported_formats;
  return true;
}
